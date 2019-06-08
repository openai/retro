/****************************************************************************
 *  wrap_wiiuse.c
 *
 *  libwiiuse wrapper for 3rd-party Classic Controller & Wii U Pro Controller support
 *
 *  Copyright (C) 2015 Eke-Eke, based on technical info & code from Tueidj 
 * 
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation and/or
 *     other materials provided with the distribution.
 *
 *  3. The name of the author may not be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#ifdef HW_RVL

#include <string.h>
#include <wiiuse/wiiuse.h>

#define WM_EXP_MEM_BASE   0x04A40000

#define WM_CTRL_STATUS_BYTE1_ATTACHMENT   0x02

#define WIIMOTE_STATE_WIIU_PRO_FOUND      0x00002
#define WIIMOTE_STATE_HANDSHAKE           0x00004
#define WIIMOTE_STATE_HANDSHAKE_COMPLETE  0x00008

extern void __real_wiiuse_handshake(struct wiimote_t *wm,ubyte *data,uword len);
extern void __real_wiiuse_set_ir(struct wiimote_t *wm,int status);
extern int __real_classic_ctrl_handshake(struct wiimote_t* wm, struct classic_ctrl_t* cc, ubyte* data, uword len);

static ubyte id[2];

void __wrap_wiiuse_handshake(struct wiimote_t *wm,ubyte *data,uword len)
{
  if (!wm->handshake_state)
  {
    if (len==0)
    {
      /* 1st step : read wiimote status (this will also starts expansion handshake if one is connected) */
      wiiuse_set_leds(wm,WIIMOTE_LED_NONE,NULL);
      wiiuse_status(wm,__wrap_wiiuse_handshake);
      return;
    }

    if (len > 2)
    {
      /* 2nd step: read expansion controller ID (if connected) */
      if (data[2]&WM_CTRL_STATUS_BYTE1_ATTACHMENT)
      {
        wiiuse_read_data(wm,id,WM_EXP_MEM_BASE+0xFE,2,__wrap_wiiuse_handshake);
        return;
      }
    }
    else 
    {
      /* 3rd step: finish handshake without trying to read internal memory if Wii U pro controller is detected */
      if ((data[0]==0x01) && (data[1]==0x20))
      {
        int *state = (int *)&wm->state;
        WIIUSE_EVENT_TYPE *event = (WIIUSE_EVENT_TYPE *)&wm->event;
        *state &= ~WIIMOTE_STATE_HANDSHAKE;
        *state |= (WIIMOTE_STATE_HANDSHAKE_COMPLETE | WIIMOTE_STATE_WIIU_PRO_FOUND);
        *event = WIIUSE_CONNECT;
        wiiuse_status(wm,NULL);
			  return;
      }
    }
  }

  /* proceed with default handshake sequence */
  __real_wiiuse_handshake(wm,data,len);
}

void __wrap_wiiuse_set_ir(struct wiimote_t *wm,int status)
{
  /* do not enable IR on Wii U Pro Controller */
  if (wm->state & WIIMOTE_STATE_WIIU_PRO_FOUND)
  {
    /* this will force data report modes 0x34 or 0x35 */
    wiiuse_status(wm,NULL);
    return;
  }

  __real_wiiuse_set_ir(wm,status);
}

int __wrap_classic_ctrl_handshake(struct wiimote_t* wm, struct classic_ctrl_t* cc, ubyte* data, uword len)
{
  /* detect Wii U Pro Controller */
  if ((len > 223) && (data[222] == 0x01) && (data[223] == 0x20))
  {
    /* set default calibration values */
    cc->btns = 0;
    cc->btns_held = 0;
    cc->btns_released = 0;
    cc->ljs.max.x = cc->ljs.max.y = cc->rjs.max.x = cc->rjs.max.y = 255;
    cc->ljs.min.x = cc->ljs.min.y = cc->rjs.min.x = cc->rjs.min.y = 0;
    cc->ljs.center.x = cc->ljs.center.y = cc->rjs.center.x = cc->rjs.center.y = 128;

    /* finish expansion handshake */
    int *type = (int *)&wm->exp.type;
    *type = EXP_CLASSIC;
    return 1;
  }

  /* sometimes the first 16 bytes are invalid but the next 16 bytes are ok (?) */
  if (data[0] == 0xFF)
  {
    if (data[16] != 0xFF)
      data += 16;
  }

  /* some 3rd-party classic controllers return invalid factory calibration data */
  if ((data[0] <= data[2]) || (data[1] >= data[2]) || (data[3] <= data[5]) || (data[4] >= data[5]) ||
      (data[6] <= data[8]) || (data[7] >= data[8]) || (data[9] <= data[11]) || (data[10] >= data[11]))
  {
    /* this will reset to default values */
    memset(data, 0, 16);
  }

  return __real_classic_ctrl_handshake(wm, cc, data, len);
}

void __wrap_classic_ctrl_event(struct classic_ctrl_t* cc, ubyte* msg)
{
	/* save previously pressed buttons */
	cc->btns_last = cc->btns;

  /* detect Wii U pro controller */
  if (cc->rjs.max.x == 0xFF)
  {
    /* pressed buttons (0 is active, 1 is inactive) */
	  cc->btns = ~((msg[8] << 8) | msg[9]) & CLASSIC_CTRL_BUTTON_ALL;

    /* convert 12-bit little endian analog joysticks position to 8-bit values */
    cc->ljs.pos.x = (msg[0] >> 4) | (msg[1] << 4);
    cc->rjs.pos.x = (msg[2] >> 4) | (msg[3] << 4);
    cc->ljs.pos.y = (msg[4] >> 4) | (msg[5] << 4);
    cc->rjs.pos.y = (msg[6] >> 4) | (msg[7] << 4);

	  /* left/right triggers (not analog) */
    cc->ls_raw = cc->btns & CLASSIC_CTRL_BUTTON_FULL_L ? 0x1F : 0;
    cc->rs_raw = cc->btns & CLASSIC_CTRL_BUTTON_FULL_R ? 0x1F : 0;
  }
  else
  {
    /* pressed buttons (0 is active, 1 is inactive) */
	  cc->btns = ~((msg[4] << 8) | msg[5]) & CLASSIC_CTRL_BUTTON_ALL;

    /* analog joysticks position */
    cc->ljs.pos.x = (msg[0] & 0x3F);
    cc->ljs.pos.y = (msg[1] & 0x3F);
    cc->rjs.pos.x = ((msg[0] & 0xC0) >> 3) | ((msg[1] & 0xC0) >> 5) | ((msg[2] & 0x80) >> 7);
    cc->rjs.pos.y = (msg[2] & 0x1F);

    /* left/right triggers (analog) */
    cc->ls_raw = (((msg[2] & 0x60) >> 2) | ((msg[3] & 0xE0) >> 5));
    cc->rs_raw = (msg[3] & 0x1F);
  }

	/* held buttons */
	cc->btns_held = (cc->btns & cc->btns_last);

	/* released buttons */
	cc->btns_released = (~cc->btns & cc->btns_last);
}

#endif
