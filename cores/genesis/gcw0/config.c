#include "osd.h"

t_config config;

static int config_load(void)
{
	//TODO: extract to function
	const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    
	/* open configuration file */
	char fname[MAXPATHLEN];
    sprintf (fname, "%s%s/config.ini", homedir, DEFAULT_PATH);
	FILE *fp = fopen(fname, "rb");
	if (fp)
	{
		/* check file size */
		fseek(fp, 0, SEEK_END);
		if (ftell(fp) != sizeof(config))
		{
			fclose(fp);
			return 0;
		}

		/* read file */
		fseek(fp, 0, SEEK_SET);
		fread(&config, sizeof(config), 1, fp);
		fclose(fp);
		return 1;
	}
				
	return 0;
}


void set_config_defaults(void)
{
    int i;
    /* sound options */
    config.psg_preamp     = 150;
    config.fm_preamp      = 100;
    config.hq_fm          = 1;
    config.hq_psg         = 1;
    config.filter         = 1;
    config.low_freq       = 200;
    config.high_freq      = 8000;
    config.lg             = 1.0;
    config.mg             = 1.0;
    config.hg             = 1.0;
    config.lp_range       = 0x9999; /* 0.6 in 16.16 fixed point */
    config.dac_bits       = 14;
    config.ym2413         = 1; /* = AUTO (0 = always OFF, 1 = always ON) */
    config.mono           = 0;

    /* system options */
    config.system         = 0; /* = AUTO (or SYSTEM_SG, SYSTEM_MARKIII, SYSTEM_SMS, SYSTEM_SMS2, SYSTEM_GG, SYSTEM_MD) */
    config.region_detect  = 0; /* = AUTO (1 = USA, 2 = EUROPE, 3 = JAPAN/NTSC, 4 = JAPAN/PAL) */
    config.vdp_mode       = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
    config.master_clock   = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
    config.force_dtack    = 0;
    config.addr_error     = 1;
    config.bios           = 0;
    config.lock_on        = 0; /* = OFF (can be TYPE_SK, TYPE_GG & TYPE_AR) */
    config.ntsc           = 0;
    config.lcd            = 0; /* 0.8 fixed point */

    /* display options */
    config.overscan         = 0; /* 3 = all borders (0 = no borders , 1 = vertical borders only, 2 = horizontal borders only) */
    config.gg_extra         = 0; /* 1 = show extended Game Gear screen (256x192) */
    config.render           = 0; /* 1 = double resolution output (only when interlaced mode 2 is enabled) */
    config.gcw0_fullscreen  = 1; /* 1 = use IPU scaling */
    config.keepaspectratio  = 1; /* 1 = aspect ratio correct with black bars, 0 = fullscreen without correct aspect ratio */
    config.gg_scanlines     = 1; /* 1 = use scanlines on Game Gear */
    config.smsmaskleftbar   = 1; /* 1 = Mask left bar on SMS (better for horizontal scrolling) */
    config.sl_autoresume    = 1; /* 1 = Automatically resume when saving and loading snapshots */
    config.a_stick          = 1; /* 1 = A-Stick on */
    config.lightgun_speed   = 1; /* 1 = simple speed multiplier for lightgun */
    config.gcw0_frameskip   = 0; /* 0 = off, 1 = skip alternate frames, 2 = skip 2 in 3 frames, etc. Useful for FMV in MCD. */

    /* controllers options */
    config.cursor         = 0;  /* different cursor designs */
    input.system[0]       = SYSTEM_GAMEPAD;
    input.system[1]       = SYSTEM_GAMEPAD;
    config.gun_cursor[0]  = 1;
    config.gun_cursor[1]  = 1;
    config.invert_mouse   = 0;
    for (i=0; i<MAX_INPUTS; i++)
    {
        /* autodetected control pad type */
        config.input[i].padtype = DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B;
    }
    
	config.buttons[A] 		= SDLK_LSHIFT;//x
	config.buttons[B] 		= SDLK_LALT;//b
	config.buttons[C] 		= SDLK_LCTRL;//a
	config.buttons[X] 		= SDLK_TAB;//l
	config.buttons[Y] 		= SDLK_SPACE;//y
	config.buttons[Z] 		= SDLK_BACKSPACE;//r
	config.buttons[START]           = SDLK_RETURN;//start
	config.buttons[MODE] 	        = SDLK_ESCAPE;//select
    
    /* try to restore user config */
	int loaded = config_load();
	if (!loaded) {
		printf("Default Settings restored\n");
	}
}


void config_save(void)
{
	//TODO: extract to function
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    /* open configuration file */
    char fname[MAXPATHLEN];
    sprintf (fname, "%s%s/config.ini", homedir, DEFAULT_PATH);
    //printf(fname);
    FILE *fp = fopen(fname, "wb");
    if (fp)
    {
        /* write file */
        fwrite(&config, sizeof(config), 1, fp);
        fclose(fp);
    }
}

