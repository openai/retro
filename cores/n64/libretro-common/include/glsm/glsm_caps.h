/* Copyright (C) 2010-2019 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this libretro SDK code part (glsm_caps.h).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LIBRETRO_SDK_GLSM_CAPS_H
#define LIBRETRO_SDK_GLSM_CAPS_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
   SGL_DEPTH_TEST             = 0,
   SGL_BLEND,
   SGL_POLYGON_OFFSET_FILL,
   SGL_FOG,
   SGL_CULL_FACE,
   SGL_ALPHA_TEST,
   SGL_SCISSOR_TEST,
   SGL_STENCIL_TEST,
   SGL_DEPTH_CLAMP,
   SGL_CLIP_DISTANCE0,
   SGL_DITHER,
   SGL_SAMPLE_ALPHA_TO_COVERAGE,
   SGL_SAMPLE_COVERAGE,
#ifndef HAVE_OPENGLES
   SGL_COLOR_LOGIC_OP,
#endif
   SGL_CAP_MAX
};

#ifdef __cplusplus
}
#endif

#endif // LIBRETRO_SDK_GLSM_CAPS_H
