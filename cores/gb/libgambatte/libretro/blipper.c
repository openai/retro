/*
 * Copyright (C) 2013 - Hans-Kristian Arntzen
 *
 * Permission is hereby granted, free of charge, 
 * to any person obtaining a copy of this software and
 * associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 * including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "blipper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if BLIPPER_LOG_PERFORMANCE
#include <time.h>
static double get_time(void)
{
   struct timespec tv;
   clock_gettime(CLOCK_MONOTONIC, &tv);
   return tv.tv_sec + tv.tv_nsec / 1000000000.0;
}
#endif

struct blipper
{
   blipper_long_sample_t *output_buffer;
   unsigned output_avail;
   unsigned output_buffer_samples;

   blipper_sample_t *filter_bank;

   unsigned phase;
   unsigned phases;
   unsigned phases_log2;
   unsigned taps;

   blipper_long_sample_t integrator;
   blipper_sample_t last_sample;

#if BLIPPER_LOG_PERFORMANCE
   double total_time;
   double integrator_time;
   unsigned long total_samples;
#endif

   int owns_filter;
};

void blipper_free(blipper_t *blip)
{
   if (blip)
   {
#if BLIPPER_LOG_PERFORMANCE
      fprintf(stderr, "[blipper]: Processed %lu samples, using %.6f seconds blipping and %.6f seconds integrating.\n", blip->total_samples, blip->total_time, blip->integrator_time);
#endif

      if (blip->owns_filter)
         free(blip->filter_bank);
      free(blip->output_buffer);
      free(blip);
   }
}

static double besseli0(double x)
{
   unsigned i;
   double sum = 0.0;

   double factorial = 1.0;
   double factorial_mult = 0.0;
   double x_pow = 1.0;
   double two_div_pow = 1.0;
   double x_sqr = x * x;

   /* Approximate. This is an infinite sum.
    * Luckily, it converges rather fast. */
   for (i = 0; i < 18; i++)
   {
      sum += x_pow * two_div_pow / (factorial * factorial);

      factorial_mult += 1.0;
      x_pow *= x_sqr;
      two_div_pow *= 0.25;
      factorial *= factorial_mult;
   }

   return sum;
}

static double sinc(double v)
{
   if (fabs(v) < 0.00001)
      return 1.0;
   else
      return sin(v) / v;
}

/* index range = [-1, 1) */
static double kaiser_window(double index, double beta)
{
   return besseli0(beta * sqrt(1.0 - index * index));
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static blipper_real_t *blipper_create_sinc(unsigned phases, unsigned taps,
      double cutoff, double beta)
{
   unsigned i, filter_len;
   double sidelobes, window_mod;
   blipper_real_t *filter;

   filter = (blipper_real_t*)malloc(phases * taps * sizeof(*filter));
   if (!filter)
      return NULL;

   sidelobes = taps / 2.0;
   window_mod = 1.0 / kaiser_window(0.0, beta);
   filter_len = phases * taps;
   for (i = 0; i < filter_len; i++)
   {
      double sinc_phase;
      double window_phase = (double)i / filter_len; /* [0, 1) */
      window_phase = 2.0 * window_phase - 1.0; /* [-1, 1) */
      sinc_phase = window_phase * sidelobes; /* [-taps / 2, taps / 2) */

      filter[i] = cutoff * sinc(M_PI * sinc_phase * cutoff) *
         kaiser_window(window_phase, beta) * window_mod;
   }

   return filter;
}

/* We differentiate and integrate at different sample rates.
 * Differentiation is D(z) = 1 - z^-1 and happens when delta impulses
 * are convolved. Integration step after decimation by D is 1 / (1 - z^-D).
 *
 * If our sinc filter is S(z) we'd have a response of
 * S(z) * (1 - z^-1) / (1 - z^-D) after blipping.
 *
 * Compensate by prefiltering S(z) with the inverse (1 - z^-D) / (1 - z^-1).
 * This filtering creates a finite length filter, albeit slightly longer.
 *
 * phases is the same as decimation rate. */
static blipper_real_t *blipper_prefilter_sinc(blipper_real_t *filter, unsigned phases,
      unsigned taps)
{
   unsigned i;
   float filter_amp = 0.75f / phases;
   blipper_real_t *tmp_filter;
   blipper_real_t *new_filter = (blipper_real_t*)malloc((phases * taps + phases) * sizeof(*filter));
   if (!new_filter)
      goto error;

   tmp_filter = (blipper_real_t*)realloc(filter, (phases * taps + phases) * sizeof(*filter));
   if (!tmp_filter)
      goto error;
   filter = tmp_filter;

   /* Integrate. */
   new_filter[0] = filter[0];
   for (i = 1; i < phases * taps; i++)
      new_filter[i] = new_filter[i - 1] + filter[i];
   for (i = phases * taps; i < phases * taps + phases; i++)
      new_filter[i] = new_filter[phases * taps - 1];

   taps++;

   /* Differentiate with offset of D. */
   memcpy(filter, new_filter, phases * sizeof(*filter));
   for (i = phases; i < phases * taps; i++)
      filter[i] = new_filter[i] - new_filter[i - phases];

   /* blipper_prefilter_sinc() boosts the gain of the sinc.
    * Have to compensate for this. Attenuate a bit more to ensure
    * we don't clip, especially in fixed point. */
   for (i = 0; i < phases * taps; i++)
      filter[i] *= filter_amp;

   free(new_filter);
   return filter;

error:
   free(new_filter);
   free(filter);
   return NULL;
}

/* Creates a polyphase filter bank.
 * Interleaves the filter for cache coherency and possibilities
 * for SIMD processing. */
static blipper_real_t *blipper_interleave_sinc(blipper_real_t *filter, unsigned phases,
      unsigned taps)
{
   unsigned t, p;
   blipper_real_t *new_filter = (blipper_real_t*)malloc(phases * taps * sizeof(*filter));
   if (!new_filter)
      goto error;

   for (t = 0; t < taps; t++)
      for (p = 0; p < phases; p++)
         new_filter[p * taps + t] = filter[t * phases + p];

   free(filter);
   return new_filter;

error:
   free(new_filter);
   free(filter);
   return NULL;
}

#if BLIPPER_FIXED_POINT
static blipper_sample_t *blipper_quantize_sinc(blipper_real_t *filter, unsigned taps)
{
   unsigned t;
   blipper_sample_t *filt = (blipper_sample_t*)malloc(taps * sizeof(*filt));
   if (!filt)
      goto error;

   for (t = 0; t < taps; t++)
      filt[t] = (blipper_sample_t)floor(filter[t] * 0x7fff + 0.5);

   free(filter);
   return filt;

error:
   free(filter);
   free(filt);
   return NULL;
}
#endif

blipper_sample_t *blipper_create_filter_bank(unsigned phases, unsigned taps,
      double cutoff, double beta)
{
   blipper_real_t *sinc_filter;

   /* blipper_prefilter_sinc() will add one tap.
    * To keep number of taps as expected, compensate for it here
    * to keep the interface more obvious. */
   if (taps <= 1)
      return 0;
   taps--;

   sinc_filter = blipper_create_sinc(phases, taps, cutoff, beta);
   if (!sinc_filter)
      return 0;

   sinc_filter = blipper_prefilter_sinc(sinc_filter, phases, taps);
   if (!sinc_filter)
      return 0;
   taps++;

   sinc_filter = blipper_interleave_sinc(sinc_filter, phases, taps);
   if (!sinc_filter)
      return 0;

#if BLIPPER_FIXED_POINT
   return blipper_quantize_sinc(sinc_filter, phases * taps);
#else
   return sinc_filter;
#endif
}

static unsigned log2_int(unsigned v)
{
   unsigned ret;
   v >>= 1;
   for (ret = 0; v; v >>= 1, ret++);
   return ret;
}

void blipper_reset(blipper_t *blip)
{
   blip->phase = 0;
   memset(blip->output_buffer, 0,
         (blip->output_avail + blip->taps) * sizeof(*blip->output_buffer));
   blip->output_avail = 0;
   blip->last_sample = 0;
   blip->integrator = 0;
}

blipper_t *blipper_new(unsigned taps, double cutoff, double beta,
      unsigned decimation, unsigned buffer_samples,
      const blipper_sample_t *filter_bank)
{
   blipper_t *blip = NULL;

   /* Sanity check. Not strictly required to be supported in C. */
   if ((-3 >> 2) != -1)
   {
      fprintf(stderr, "Integer right shift not supported.\n");
      return NULL;
   }

   if ((decimation & (decimation - 1)) != 0)
   {
      fprintf(stderr, "[blipper]: Decimation factor must be POT.\n");
      return NULL;
   }

   blip = (blipper_t*)calloc(1, sizeof(*blip));
   if (!blip)
      return NULL;

   blip->phases = decimation;
   blip->phases_log2 = log2_int(decimation);

   blip->taps = taps;

   if (!filter_bank)
   {
      blip->filter_bank = blipper_create_filter_bank(blip->phases, taps, cutoff, beta);
      if (!blip->filter_bank)
         goto error;
      blip->owns_filter = 1;
   }
   else
      blip->filter_bank = (blipper_sample_t*)filter_bank;

   blip->output_buffer = (blipper_long_sample_t*)calloc(buffer_samples + blip->taps,
         sizeof(*blip->output_buffer));
   if (!blip->output_buffer)
      goto error;
   blip->output_buffer_samples = buffer_samples + blip->taps;

   return blip;

error:
   blipper_free(blip);
   return NULL;
}

void blipper_push_delta(blipper_t *blip, blipper_long_sample_t delta, unsigned clocks_step)
{
   unsigned target_output, filter_phase, taps, i;
   const blipper_sample_t *response;
   blipper_long_sample_t *target;

   blip->phase += clocks_step;

   target_output = (blip->phase + blip->phases - 1) >> blip->phases_log2;

   filter_phase = (target_output << blip->phases_log2) - blip->phase;
   response = blip->filter_bank + blip->taps * filter_phase;

   target = blip->output_buffer + target_output;
   taps = blip->taps;

   for (i = 0; i < taps; i++)
      target[i] += delta * response[i];

   blip->output_avail = target_output;
}

void blipper_push_samples(blipper_t *blip, const blipper_sample_t *data,
      unsigned samples, unsigned stride)
{
   unsigned s;
   unsigned clocks_skip = 0;
   blipper_sample_t last = blip->last_sample;

#if BLIPPER_LOG_PERFORMANCE
   double t0 = get_time();
#endif

   for (s = 0; s < samples; s++, data += stride)
   {
      blipper_sample_t val = *data;
      if (val != last)
      {
         blipper_push_delta(blip, (blipper_long_sample_t)val - (blipper_long_sample_t)last, clocks_skip + 1);
         clocks_skip = 0;
         last = val;
      }
      else
         clocks_skip++;
   }

   blip->phase += clocks_skip;
   blip->output_avail = (blip->phase + blip->phases - 1) >> blip->phases_log2;
   blip->last_sample = last;

#if BLIPPER_LOG_PERFORMANCE
   blip->total_time += get_time() - t0;
   blip->total_samples += samples;
#endif
}

unsigned blipper_read_avail(blipper_t *blip)
{
   return blip->output_avail;
}

void blipper_read(blipper_t *blip, blipper_sample_t *output, unsigned samples,
      unsigned stride)
{
   unsigned s;
   blipper_long_sample_t sum = blip->integrator;
   const blipper_long_sample_t *out = blip->output_buffer;

#if BLIPPER_LOG_PERFORMANCE
   double t0 = get_time();
#endif

#if BLIPPER_FIXED_POINT
   for (s = 0; s < samples; s++, output += stride)
   {
      blipper_long_sample_t quant;

      /* Cannot overflow. Also add a leaky integrator.
         Mitigates DC shift numerical instability which is
         inherent for integrators. */
      sum += (out[s] >> 1) - (sum >> 9);

      /* Rounded. With leaky integrator, this cannot overflow. */
      quant = (sum + 0x4000) >> 15;

      /* Clamp. quant can potentially have range [-0x10000, 0xffff] here.
       * In both cases, top 16-bits will have a uniform bit pattern which can be exploited. */
      if ((blipper_sample_t)quant != quant)
      {
         quant = (quant >> 16) ^ 0x7fff;
         sum = quant << 15;
      }

      *output = quant;
   }
#else
   for (s = 0; s < samples; s++, output += stride)
   {
      /* Leaky integrator, same as fixed point (1.0f / 512.0f) */
      sum += out[s] - sum * 0.00195f;
      *output = sum;
   }
#endif

   /* Don't bother with ring buffering.
    * The entire buffer should be read out ideally anyways. */
   memmove(blip->output_buffer, blip->output_buffer + samples,
         (blip->output_avail + blip->taps - samples) * sizeof(*out));
   memset(blip->output_buffer + blip->taps, 0, samples * sizeof(*out));
   blip->output_avail -= samples;
   blip->phase -= samples << blip->phases_log2;

   blip->integrator = sum;

#if BLIPPER_LOG_PERFORMANCE
   blip->integrator_time += get_time() - t0;
#endif
}

