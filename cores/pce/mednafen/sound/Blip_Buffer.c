// Blip_Buffer 0.4.1. http://www.slack.net/~ant/

#include <blip/Blip_Buffer.h>

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef ULLONG_MAX
#define ULLONG_MAX 18446744073709551615ULL
#endif

/* Copyright (C) 2003-2006 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#ifdef BLARGG_ENABLE_OPTIMIZER
#include BLARGG_ENABLE_OPTIMIZER
#endif

void Blip_Buffer_init(Blip_Buffer* bbuf)
{
   bbuf->factor       = (blip_u64)ULLONG_MAX;
   bbuf->offset       = 0;
   bbuf->buffer       = 0;
   bbuf->buffer_size  = 0;
   bbuf->sample_rate  = 0;
   bbuf->reader_accum = 0;
   bbuf->bass_shift   = 0;
   bbuf->clock_rate   = 0;
   bbuf->bass_freq    = 16;
   bbuf->length       = 0;
}

void Blip_Buffer_deinit(Blip_Buffer* bbuf)
{
   if (bbuf->buffer)
      free(bbuf->buffer);
}

void Blip_Buffer_clear(Blip_Buffer* bbuf, int entire_buffer)
{
   bbuf->offset      = 0;
   bbuf->reader_accum = 0;
   bbuf->modified    = 0;
   if (bbuf->buffer)
   {
      long count = (entire_buffer ? bbuf->buffer_size : Blip_Buffer_samples_avail(bbuf));
      memset(bbuf->buffer, 0, (count + blip_buffer_extra_) * sizeof(blip_buf_t_));
   }
}

blargg_err_t Blip_Buffer_set_sample_rate(Blip_Buffer* bbuf, long new_rate,
      int msec)
{
   // start with maximum length that resampled time can represent
   blip_s64 new_size = (ULLONG_MAX >> BLIP_BUFFER_ACCURACY) - blip_buffer_extra_ -
                       64;

   // simple safety check, since code elsewhere may not be safe for sizes approaching (2 ^ 31).
   if (new_size > ((1LL << 30) - 1))
      new_size = (1LL << 30) - 1;

   if (msec != 0)
   {
      blip_s64 s = ((blip_s64)new_rate * (msec + 1) + 999) / 1000;
      if (s < new_size)
         new_size = s;
      else
         assert(0);   // fails if requested buffer length exceeds limit
   }

   if (bbuf->buffer_size != new_size)
   {
      void* p = realloc(bbuf->buffer, (new_size + blip_buffer_extra_) * sizeof (bbuf->buffer));
      if (!p)
         return "Out of memory";

      bbuf->buffer = (blip_buf_t_*) p;
   }

   bbuf->buffer_size = new_size;

   // update things based on the sample rate
   bbuf->sample_rate = new_rate;
   bbuf->length = new_size * 1000 / new_rate - 1;
   if (msec)
      assert(bbuf->length == msec);   // ensure length is same as that passed in
   if (bbuf->clock_rate)
      Blip_Buffer_set_clock_rate(bbuf, bbuf->clock_rate);
   Blip_Buffer_bass_freq(bbuf, bbuf->bass_freq);

   Blip_Buffer_clear(bbuf, 1);

   return 0; // success
}

blip_resampled_time_t Blip_Buffer_clock_rate_factor(Blip_Buffer* bbuf,
      long rate)
{
   double ratio = (double) bbuf->sample_rate / rate;
   blip_s64 factor = (blip_s64) floor(ratio * (1LL << BLIP_BUFFER_ACCURACY) + 0.5);
   assert(factor > 0
          || !bbuf->sample_rate);   // fails if clock/output ratio is too large
   return (blip_resampled_time_t) factor;
}

void Blip_Buffer_bass_freq(Blip_Buffer* bbuf,  int freq)
{
   int shift = 31;

   bbuf->bass_freq = freq;
   if (freq > 0)
   {
      long f;
      shift = 13;
      f = (freq << 16) / bbuf->sample_rate;
      while ((f >>= 1) && --shift) { }
   }
   bbuf->bass_shift = shift;
}

void Blip_Buffer_end_frame(Blip_Buffer* bbuf,  blip_time_t t)
{
   bbuf->offset += t * bbuf->factor;
   assert(Blip_Buffer_samples_avail(bbuf) <= (long) bbuf->buffer_size);   // time outside buffer length
}

void Blip_Buffer_remove_silence(Blip_Buffer* bbuf,  long count)
{
   assert(count <=
          Blip_Buffer_samples_avail(bbuf));   // tried to remove more samples than available
   bbuf->offset -= (blip_resampled_time_t) count << BLIP_BUFFER_ACCURACY;
}

long Blip_Buffer_count_samples(Blip_Buffer* bbuf,  blip_time_t t)
{
   unsigned long last_sample  = Blip_Buffer_resampled_time(bbuf, t) >> BLIP_BUFFER_ACCURACY;
   unsigned long first_sample = bbuf->offset >> BLIP_BUFFER_ACCURACY;
   return (long)(last_sample - first_sample);
}

void Blip_Buffer_remove_samples(Blip_Buffer* bbuf,  long count)
{
   if (count)
   {
      long remain;
      Blip_Buffer_remove_silence(bbuf, count);

      // copy remaining samples to beginning and clear old samples
      remain = Blip_Buffer_samples_avail(bbuf) + blip_buffer_extra_;
      memmove(bbuf->buffer, bbuf->buffer + count, remain * sizeof(bbuf->buffer));
      memset(bbuf->buffer + remain, 0, count * sizeof(bbuf->buffer));
   }
}

long Blip_Buffer_read_samples(Blip_Buffer* bbuf, blip_sample_t* out,
                              long max_samples)
{
   long count = Blip_Buffer_samples_avail(bbuf);
   if (count > max_samples)
      count = max_samples;

   if (count)
   {
      int const bass = BLIP_READER_BASS(*bbuf);
      BLIP_READER_BEGIN(reader, *bbuf);

      blip_long n;

      for (n = count; n; --n)
      {
         blip_long s = BLIP_READER_READ(reader);
         if ((blip_sample_t) s != s)
            s = 0x7FFF - (s >> 24);
         *out = (blip_sample_t) s;
         out += 2;
         BLIP_READER_NEXT(reader, bass);
      }

      BLIP_READER_END(reader, *bbuf);

      Blip_Buffer_remove_samples(bbuf, count);
   }
   return count;
}

void Blip_Buffer_mix_samples(Blip_Buffer* bbuf, blip_sample_t const* in, long count)
{
   blip_buf_t_* out = bbuf->buffer + (bbuf->offset >> BLIP_BUFFER_ACCURACY) +
                 blip_widest_impulse_ / 2;

   int const sample_shift = blip_sample_bits - 16;
   int prev = 0;
   while (count--)
   {
      blip_long s = (blip_long) * in++ << sample_shift;
      *out += s - prev;
      prev = s;
      ++out;
   }
   *out -= prev;
}
