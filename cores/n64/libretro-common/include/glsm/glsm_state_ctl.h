/* Copyright (C) 2010-2019 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this libretro SDK code part (glsm_state_ctl.h).
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

#ifndef LIBRETRO_SDK_GLSM_STATE_CTL_H
#define LIBRETRO_SDK_GLSM_STATE_CTL_H

#ifdef __cplusplus
extern "C" {
#endif

enum glsm_state_ctl
{
   GLSM_CTL_NONE = 0,
   GLSM_CTL_STATE_SETUP,
   GLSM_CTL_STATE_BIND,
   GLSM_CTL_STATE_UNBIND,
   GLSM_CTL_STATE_CONTEXT_RESET,
   GLSM_CTL_STATE_CONTEXT_DESTROY,
   GLSM_CTL_STATE_CONTEXT_INIT,
   GLSM_CTL_IS_IMM_VBO,
   GLSM_CTL_SET_IMM_VBO,
   GLSM_CTL_UNSET_IMM_VBO,
   GLSM_CTL_IMM_VBO_DISABLE,
   GLSM_CTL_IMM_VBO_DRAW,
   GLSM_CTL_PROC_ADDRESS_GET
};

bool glsm_ctl(enum glsm_state_ctl state, void *data);

#ifdef __cplusplus
}
#endif

#endif // LIBRETRO_SDK_GLSM_STATE_CTL_H
