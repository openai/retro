/*
 * Convoluted Cosine Resampler
 * Copyright (C) 2014 - Bouhlel Ali ( aliaspider@gmail.com )
 *
 * licence: GPLv3
 */

#ifndef CC_RESAMPLER_H
#define CC_RESAMPLER_H

#ifdef CC_RESAMPLER

#ifdef __cplusplus
extern "C" {
#endif

typedef struct audio_frame
{
   int16_t l;
   int16_t r;
}audio_frame_t;

#define CC_DECIMATION_RATE 32

static unsigned int CC_accumulated_samples = 0;
static unsigned int CC_write_pos = 0;

#ifdef _MIPS_ARCH_ALLEGREX
static void CC_init(void)
{
   CC_accumulated_samples = 0;
   CC_write_pos = 0;

   __asm__ (
   ".set    push                   \n"
   ".set    noreorder              \n"
   "vzero.q c030                   \n"
#ifndef CC_RESAMPLER_NO_HIGHPASS
   "vzero.p c020                   \n"
   "vmul.s  s022, s022[2], s022[2] \n"
   "vmul.s  s022, s022   , s022[3] \n"
   "vadd.s  s022, s022   , s022[2] \n" // s022 = 14
   "vexp2.s s022, s022[-X]         \n" // s022 = 2^-14
#endif
   ".set    pop                    \n"
   );

}

static void CC_renderaudio(audio_frame_t* sound_buf, unsigned samples)
{
   static const float CC_kernel[64]=
   {
      0.00000000000000, 0.00006103515625, 0.00030517578125, 0.00067138671875,
      0.00115966796875, 0.00183105468750, 0.00262451171875, 0.00354003906250,
      0.00457763671875, 0.00567626953125, 0.00689697265625, 0.00823974609375,
      0.00964355468750, 0.01104736328125, 0.01257324218750, 0.01403808593750,
      0.01562500000000, 0.01715087890625, 0.01861572265625, 0.02014160156250,
      0.02154541015625, 0.02294921875000, 0.02429199218750, 0.02551269531250,
      0.02661132812500, 0.02764892578125, 0.02856445312500, 0.02935791015625,
      0.03002929687500, 0.03051757812500, 0.03088378906250, 0.03112792968750,
      0.03118896484375, 0.03112792968750, 0.03088378906250, 0.03051757812500,
      0.03002929687500, 0.02935791015625, 0.02856445312500, 0.02764892578125,
      0.02661132812500, 0.02551269531250, 0.02429199218750, 0.02294921875000,
      0.02154541015625, 0.02014160156250, 0.01861572265625, 0.01715087890625,
      0.01562500000000, 0.01403808593750, 0.01257324218750, 0.01104736328125,
      0.00964355468750, 0.00823974609375, 0.00689697265625, 0.00567626953125,
      0.00457763671875, 0.00354003906250, 0.00262451171875, 0.00183105468750,
      0.00115966796875, 0.00067138671875, 0.00030517578125, 0.00006103515625
   };

   static uint32_t out_buf[512];
   unsigned i;

   for (i=0; i!=samples; i++)
   {
      __asm__ (
      ".set    push                                 \n"
      ".set    noreorder                            \n"

      "lv.s    s000,   0(%1)                        \n"   // s000 = sound_buf[i]
      "lv.s    s002,   0(%0)                        \n"   // s002 = CC_kernel[CC_accumulated_samples]
      "lv.s    s003, 128(%0)                        \n"   // s003 = CC_kernel[CC_accumulated_samples + 32]

      "vs2i.s  c000, s000                           \n"   // s000 = sound_buf[i].l<<16    , s001 = sound_buf[i].r << 16
      "vi2f.p  c000, c000,  16                      \n"   // s000 = (float) sound_buf[i].l, s001 = (float) sound_buf[i].r
      "vmul.q  c000, c000[X,Y,X,Y], c000[W,W,Z,Z]   \n"   // c000 : current.l, current.r, next.l, next.r
      "vadd.q  c030, c030, c000                     \n"   // c030 : current.l, current.r, next.l, next.r (accumulated)

      ".set         pop\n"
      ::"r" (CC_kernel+CC_accumulated_samples),"r" (sound_buf+i)
      );

      CC_accumulated_samples++;
      if (CC_accumulated_samples == 32)
      {
         CC_accumulated_samples = 0;

         __asm__ (
         ".set    push                  \n"
         ".set    noreorder             \n"

#ifdef CC_RESAMPLER_NO_HIGHPASS
         "vf2in.p c000, c030, 16        \n"
#else
         "vsub.p  c000, c030, c020      \n"
         "vmul.p  c002, c000, c022[X,X] \n"
         "vadd.p  c020, c020, c002      \n"
         "vf2in.p c000, c000, 16        \n"
#endif

         "vi2s.p  s000, c000            \n"
         "sv.s    s000,   %0            \n"
         "vmov.q  c030, c030[Z,W,0,0]   \n"

         ".set    pop\n"
         :"=m"(out_buf[CC_write_pos++])
         );

         if (CC_write_pos == 512)
         {
            audio_batch_cb((int16_t*)out_buf, 512);
            CC_write_pos = 0;
         }
      }
   }
}
#else

#ifndef CC_RESAMPLER_NO_HIGHPASS
static int16_t CC_prevsmpl_l = 0;
static int16_t CC_prevsmpl_r = 0;
static int32_t CC_highpass_l = 0;
static int32_t CC_highpass_r = 0;
#endif

static int32_t CC_current_l = 0;
static int32_t CC_current_r = 0;
static int32_t CC_next_l    = 0;
static int32_t CC_next_r    = 0;

static void CC_init(void)
{
#ifndef CC_RESAMPLER_NO_HIGHPASS
   CC_prevsmpl_l = 0;
   CC_prevsmpl_r = 0;
   CC_highpass_l = 0;
   CC_highpass_r = 0;
#endif
   CC_accumulated_samples = 0;
   CC_write_pos = 0;
   CC_current_l = 0;
   CC_current_r = 0;
   CC_next_l    = 0;
   CC_next_r    = 0;
}

static void CC_renderaudio(audio_frame_t* sound_buf, unsigned samples)
{
   static const int16_t CC_kernel[32]=
   {
      0x0800, 0x07fb, 0x07eb, 0x07d3, 0x07b3, 0x0787, 0x0753, 0x0717,
      0x06d3, 0x068b, 0x063b, 0x05e2, 0x0586, 0x052a, 0x04c6, 0x0466,
      0x0402, 0x0399, 0x0339, 0x02d5, 0x0279, 0x021d, 0x01c4, 0x0174,
      0x012c, 0x00e8, 0x00ac, 0x0078, 0x004c, 0x002c, 0x0014, 0x0004
   };
   static const int16_t CC_kernel_r[32]=
   {
      0x0000, 0x0004, 0x0014, 0x002c, 0x004c, 0x0078, 0x00ac, 0x00e8,
      0x012c, 0x0174, 0x01c4, 0x021d, 0x0279, 0x02d5, 0x0339, 0x0399,
      0x0402, 0x0466, 0x04c6, 0x052a, 0x0586, 0x05e2, 0x063b, 0x068b,
      0x06d3, 0x0717, 0x0753, 0x0787, 0x07b3, 0x07d3, 0x07eb, 0x07fb
   };

   static int16_t out_buf[2048];
   unsigned i;

   unsigned int accumulated_samples = CC_accumulated_samples;
   unsigned int write_pos = CC_write_pos;
   int32_t current_l = CC_current_l;
   int32_t current_r = CC_current_r;
   int32_t next_l    = CC_next_l;
   int32_t next_r    = CC_next_r;

   for (i=0; i!=samples; i++)
   {
      current_l += sound_buf[i].l * CC_kernel[accumulated_samples];
      current_r += sound_buf[i].r * CC_kernel[accumulated_samples];
      next_l    += sound_buf[i].l * CC_kernel_r[accumulated_samples];
      next_r    += sound_buf[i].r * CC_kernel_r[accumulated_samples];

      accumulated_samples++;
      if (accumulated_samples == 32)
      {
#ifdef CC_RESAMPLER_NO_HIGHPASS
         out_buf[write_pos++] = (current_l>>16);
         out_buf[write_pos++] = (current_r>>16);
#else
         int16_t sample_l = (current_l>>16);
         int32_t tmp_l = CC_highpass_l + ((sample_l-CC_prevsmpl_l)<<8);
         CC_highpass_l = tmp_l - (tmp_l>>8);
         CC_prevsmpl_l = sample_l;
         out_buf[write_pos++] = ((CC_highpass_l+128)>>8);

         int16_t sample_r = (current_r>>16);
         int32_t tmp_r = CC_highpass_r + ((sample_r-CC_prevsmpl_r)<<8);
         CC_highpass_r = tmp_r - (tmp_r>>8);
         CC_prevsmpl_r = sample_r;
         out_buf[write_pos++] = ((CC_highpass_r+128)>>8);
#endif
         accumulated_samples = 0;
         current_l = next_l;
         current_r = next_r;
         next_l = 0;
         next_r = 0;

         if (write_pos == 2048)
         {
            audio_batch_cb(out_buf, 1024);
            write_pos = 0;
         }
      }
   }
   CC_accumulated_samples = accumulated_samples;
   CC_write_pos = write_pos;
   CC_current_l = current_l;
   CC_current_r = current_r;
   CC_next_l    = next_l;
   CC_next_r    = next_r;
}
#endif // _MIPS_ARCH_ALLEGREX

#ifdef __cplusplus
}
#endif

#endif // CC_RESAMPLER
#endif // CC_RESAMPLER_H
