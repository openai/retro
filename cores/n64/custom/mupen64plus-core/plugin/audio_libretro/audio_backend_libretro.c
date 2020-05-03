/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - audio_backend_compat.c                                  *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2014 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "api/m64p_types.h"
#include <libretro.h>
#include "device/rcp/ai/ai_controller.h"
#include "../../../../mupen64plus-core/src/main/main.h"
#include "../../../../mupen64plus-core/src/device/device.h"
#include "../../../../mupen64plus-core/src/main/rom.h"
#include "plugin/plugin.h"
#include "device/rcp/ri/ri_controller.h"
#include "device/rcp/vi/vi_controller.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <audio/conversion/float_to_s16.h>
#include <audio/conversion/s16_to_float.h>
#include <audio/audio_resampler.h>

extern retro_audio_sample_batch_t audio_batch_cb;

static unsigned MAX_AUDIO_FRAMES = 2048;

#define VI_INTR_TIME 500000

/* Read header for type definition */
static int GameFreq = 33600;
static unsigned CountsPerSecond;
static unsigned BytesPerSecond;
static unsigned CountsPerByte;

static const retro_resampler_t *resampler;
static void *resampler_audio_data;
static float *audio_in_buffer_float;
static float *audio_out_buffer_float;
static int16_t *audio_out_buffer_s16;

void (*audio_convert_s16_to_float_arm)(float *out,
      const int16_t *in, size_t samples, float gain);
void (*audio_convert_float_to_s16_arm)(int16_t *out,
      const float *in, size_t samples);

void deinit_audio_libretro(void)
{
   if (resampler && resampler_audio_data)
   {
      resampler->free(resampler_audio_data);
      resampler = NULL;
      resampler_audio_data = NULL;
      free(audio_in_buffer_float);
      free(audio_out_buffer_float);
      free(audio_out_buffer_s16);
   }
}

void init_audio_libretro(unsigned max_audio_frames)
{
   retro_resampler_realloc(&resampler_audio_data, &resampler, "sinc", RESAMPLER_QUALITY_DONTCARE, 1.0);

   MAX_AUDIO_FRAMES = max_audio_frames;

   audio_in_buffer_float  = malloc(2 * MAX_AUDIO_FRAMES * sizeof(float));
   audio_out_buffer_float = malloc(2 * MAX_AUDIO_FRAMES * sizeof(float));
   audio_out_buffer_s16   = malloc(2 * MAX_AUDIO_FRAMES * sizeof(int16_t));

   convert_s16_to_float_init_simd();
   convert_float_to_s16_init_simd();
}

static void aiDacrateChanged(void *user_data, unsigned int frequency, unsigned int bits)
{
   GameFreq        = frequency;
   BytesPerSecond  = frequency * 4;
   CountsPerSecond = VI_INTR_TIME * 60 /* TODO/FIXME - dehardcode */;
   CountsPerByte   = CountsPerSecond / BytesPerSecond;

#if 0
   printf("CountsPerByte: %d, GameFreq: %d\n", CountsPerByte, GameFreq);
#endif
}

/* A fully compliant implementation is not really possible with just the zilmar spec.
 * We assume bits == 16 (assumption compatible with audio-sdl plugin implementation)
 */
void set_audio_format_via_libretro(void* user_data,
      unsigned int frequency, unsigned int bits)
{
   struct ai_controller* ai = (struct ai_controller*)user_data;
   uint32_t saved_ai_dacrate = ai->regs[AI_DACRATE_REG];

   /* notify plugin of the new frequency (can't do the same for bits) */
   ai->regs[AI_DACRATE_REG] = ai->vi->clock / frequency - 1;

   aiDacrateChanged(user_data, frequency, bits);

   /* restore original registers values */
   ai->regs[AI_DACRATE_REG] = saved_ai_dacrate;
}

static void aiLenChanged(void* user_data, const void* buffer, size_t size)
{
   size_t max_frames, remain_frames;
   uint32_t i;
   double ratio;
   struct resampler_data data = {0};
   int16_t *out      = NULL;
   int16_t *raw_data = (int16_t*)buffer;
   size_t frames     = size / 4;
   uint8_t *p        = (uint8_t*)buffer;

   for (i = 0; i < size; i += 4)
   {
      p[i ] ^= p[i + 2];
      p[i + 2] ^= p[i ];
      p[i ] ^= p[i + 2];
      p[i + 1] ^= p[i + 3];
      p[i + 3] ^= p[i + 1];
      p[i + 1] ^= p[i + 3];
   }

audio_batch:
   out               = NULL;
   ratio             = 44100.0 / GameFreq;
   max_frames        = (GameFreq > 44100) ? MAX_AUDIO_FRAMES : (size_t)(MAX_AUDIO_FRAMES / ratio - 1);
   remain_frames     = 0;

   if (frames > max_frames)
   {
      remain_frames = frames - max_frames;
      frames = max_frames;
   }

   data.data_in      = audio_in_buffer_float;
   data.data_out     = audio_out_buffer_float;
   data.input_frames = frames;
   data.ratio        = ratio;

   convert_s16_to_float(audio_in_buffer_float, raw_data, frames * 2, 1.0f);
   resampler->process(resampler_audio_data, &data);
   convert_float_to_s16(audio_out_buffer_s16, audio_out_buffer_float, data.output_frames * 2);

   out                    = audio_out_buffer_s16;

   while (data.output_frames)
   {
      size_t ret          = audio_batch_cb(out, data.output_frames);
      data.output_frames -= ret;
      out                += ret * 2;
   }
   if (remain_frames)
   {
      raw_data = raw_data + frames * 2;
      frames   = remain_frames;
      goto audio_batch;
   }
}

/* Abuse core & audio plugin implementation details to obtain the desired effect. */
void push_audio_samples_via_libretro(void* user_data, const void* buffer, size_t size)
{
   struct ai_controller* ai = (struct ai_controller*)user_data;
   uint32_t saved_ai_length = ai->regs[AI_LEN_REG];
   uint32_t saved_ai_dram = ai->regs[AI_DRAM_ADDR_REG];

   /* notify plugin of new samples to play.
    * Exploit the fact that buffer points in ai->ri->rdram.dram to retrieve dram_addr_reg value */
   ai->regs[AI_DRAM_ADDR_REG] = (uint8_t*)buffer - (uint8_t*)g_dev.ri.rdram->dram;
   ai->regs[AI_LEN_REG] = size;

   aiLenChanged(user_data, buffer, size);

   /* restore original registers vlaues */
   ai->regs[AI_LEN_REG]       = saved_ai_length;
   ai->regs[AI_DRAM_ADDR_REG] = saved_ai_dram;
}
