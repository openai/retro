
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <pwd.h>

/****************************************************************************
 * Config Option
 *
 ****************************************************************************/
 
enum {A = 0, B, C, X, Y, Z, START, MODE};
 
typedef struct
{
  uint8 padtype;
} t_input_config;

typedef struct
{
  uint8 hq_fm;
  uint8 filter;
  uint8 hq_psg;
  uint8 dac_bits;
  uint8 ym2413;
  int16 psg_preamp;
  int16 fm_preamp;
  uint32 lp_range;
  int16 low_freq;
  int16 high_freq;
  int16 lg;
  int16 mg;
  int16 hg;
  uint8 mono;
  uint8 system;
  uint8 region_detect;
  uint8 vdp_mode;
  uint8 master_clock;
  uint8 force_dtack;
  uint8 addr_error;
  uint8 bios;
  uint8 lock_on;
  uint8 hot_swap;
  uint8 invert_mouse;
  uint8 gun_cursor[2];
  uint8 overscan;
  uint8 gg_extra;
  uint8 ntsc;
  uint8 lcd;
  uint8 render;
  t_input_config input[MAX_INPUTS];
  uint8 gcw0_fullscreen;
  uint8 gcw0_frameskip;
  uint8 keepaspectratio;
  uint8 gg_scanlines;
  uint8 smsmaskleftbar;
  uint8 sl_autoresume;
  uint8 lightgun_speed;
  uint8 a_stick;
  uint8 cursor;
  SDLKey buttons[8];
  //SDLKey button_a;
  //SDLKey button_b;
  //SDLKey button_c;
  //SDLKey button_x;
  //SDLKey button_y;
  //SDLKey button_z;
  //SDLKey button_start;
  //SDLKey button_mode;
} t_config;

/* Global variables */
extern t_config config;
extern void config_save(void);
extern void set_config_defaults(void);

#endif /* _CONFIG_H_ */

