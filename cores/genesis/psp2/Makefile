
# Makefile for genplus SDL
#
# (c) 1999, 2000, 2001, 2002, 2003  Charles MacDonald
# modified by Eke-Eke <eke_eke31@yahoo.fr>
#
# Defines :
# -DLSB_FIRST : for little endian systems.
# -DLOGERROR  : enable message logging
# -DLOGVDP    : enable VDP debug messages
# -DLOGSOUND  : enable AUDIO debug messages
# -DLOG_SCD   : enable SCD debug messages
# -DLOG_CDD   : enable CDD debug messages
# -DLOG_CDC   : enable CDC debug messages
# -DLOG_PCM   : enable PCM debug messages
# -DLOGSOUND  : enable AUDIO debug messages
# -D8BPP_RENDERING  - configure for 8-bit pixels (RGB332)
# -D15BPP_RENDERING - configure for 15-bit pixels (RGB555)
# -D16BPP_RENDERING - configure for 16-bit pixels (RGB565)
# -D32BPP_RENDERING - configure for 32-bit pixels (RGB888)

NAME	  = gen_vita
PSP_APP_NAME=GENPLUSGXVITA
PSP_APP_VER=1.7.5

CC        = arm-vita-eabi-gcc
CFLAGS    = -O3 -fomit-frame-pointer -Wall -Wno-strict-aliasing -ansi -std=c11 \
-pedantic-errors -fno-unwind-tables -fno-asynchronous-unwind-tables -ftree-vectorize \
-mfloat-abi=hard -ffast-math -fsingle-precision-constant -ftree-vectorizer-verbose=2 -fopt-info-vec-optimized -funroll-loops
#-g -ggdb -pg
#-fomit-frame-pointer
LDFLAGS   = -Wl,-q
DEFINES   = -DPSP_APP_NAME=\"$(PSP_APP_NAME)\" -DPSP_APP_VER=\"$(PSP_APP_VER)\" \
						-DLSB_FIRST -DUSE_15BPP_RENDERING -DUSE_LIBTREMOR -DALT_RENDERER -DALIGN_LONG -DHAVE_ALLOCA_H -DUSE_ABGR
SRCDIR    = ../core
INCLUDES  = -I$(SRCDIR) -I$(SRCDIR)/z80 -I$(SRCDIR)/m68k -I$(SRCDIR)/sound -I$(SRCDIR)/input_hw -I$(SRCDIR)/cart_hw -I$(SRCDIR)/cart_hw/svp -I$(SRCDIR)/cd_hw -I$(SRCDIR)/ntsc -I$(SRCDIR)/tremor -I$(SRCDIR)/../psp2
LIBS	  = -lpsplib -lvita2d  -lfreetype -lpng -lz -lm -lSceDisplay_stub -lSceGxm_stub 	\
	-lSceCtrl_stub -lSceAudio_stub -lSceRtc_stub -lScePower_stub -lSceAppUtil_stub
#-ldebugnet -lSceNet_stub -lSceNetCtl_stub

OBJDIR = ./build_vita

OBJECTS	=       $(OBJDIR)/z80.o

OBJECTS	+=     	$(OBJDIR)/m68kcpu.o \
		$(OBJDIR)/s68kcpu.o

OBJECTS	+=     	$(OBJDIR)/genesis.o	 \
		$(OBJDIR)/vdp_ctrl.o	 \
		$(OBJDIR)/vdp_render.o   \
		$(OBJDIR)/system.o       \
		$(OBJDIR)/io_ctrl.o	 \
		$(OBJDIR)/mem68k.o	 \
		$(OBJDIR)/memz80.o	 \
		$(OBJDIR)/membnk.o	 \
		$(OBJDIR)/state.o        \
		$(OBJDIR)/loadrom.o

OBJECTS	+=      $(OBJDIR)/input.o	  \
		$(OBJDIR)/gamepad.o	  \
		$(OBJDIR)/lightgun.o	  \
		$(OBJDIR)/mouse.o	  \
		$(OBJDIR)/activator.o	  \
		$(OBJDIR)/xe_1ap.o	  \
		$(OBJDIR)/teamplayer.o    \
		$(OBJDIR)/paddle.o	  \
		$(OBJDIR)/sportspad.o     \
		$(OBJDIR)/terebi_oekaki.o \
		$(OBJDIR)/graphic_board.o

OBJECTS	+=      $(OBJDIR)/sound.o	\
		$(OBJDIR)/psg.o     \
		$(OBJDIR)/ym2413.o      \
		$(OBJDIR)/ym2612.o

OBJECTS	+=	$(OBJDIR)/blip_buf.o

OBJECTS	+=	$(OBJDIR)/eq.o

OBJECTS	+=      $(OBJDIR)/sram.o        \
		$(OBJDIR)/svp.o	        \
		$(OBJDIR)/ssp16.o       \
		$(OBJDIR)/ggenie.o      \
		$(OBJDIR)/areplay.o	\
		$(OBJDIR)/eeprom_93c.o  \
		$(OBJDIR)/eeprom_i2c.o  \
		$(OBJDIR)/eeprom_spi.o  \
		$(OBJDIR)/md_cart.o	\
		$(OBJDIR)/sms_cart.o

OBJECTS	+=      $(OBJDIR)/scd.o	\
		$(OBJDIR)/cdd.o	\
		$(OBJDIR)/cdc.o	\
		$(OBJDIR)/gfx.o	\
		$(OBJDIR)/pcm.o	\
		$(OBJDIR)/cd_cart.o

OBJECTS	+=	$(OBJDIR)/sms_ntsc.o	\
		$(OBJDIR)/md_ntsc.o

OBJECTS	+=	$(OBJDIR)/main.o	\
		$(OBJDIR)/emumain.o	\
		$(OBJDIR)/menu.o	\
		$(OBJDIR)/config.o	\
		$(OBJDIR)/error.o	\
		$(OBJDIR)/unzip.o       \
		$(OBJDIR)/fileio.o

OBJECTS	+=	$(OBJDIR)/bitwise.o	 \
		$(OBJDIR)/block.o      \
		$(OBJDIR)/codebook.o   \
		$(OBJDIR)/floor0.o     \
		$(OBJDIR)/floor1.o     \
		$(OBJDIR)/framing.o    \
		$(OBJDIR)/info.o       \
		$(OBJDIR)/mapping0.o   \
		$(OBJDIR)/mdct.o       \
		$(OBJDIR)/registry.o   \
		$(OBJDIR)/res012.o     \
		$(OBJDIR)/sharedbook.o \
		$(OBJDIR)/synthesis.o  \
		$(OBJDIR)/vorbisfile.o \
		$(OBJDIR)/window.o


all: $(NAME).velf

$(NAME).velf: $(NAME).elf
	#advice from xyzz strip before create elf
		arm-vita-eabi-strip -g $<
	#i put db.json there use your location
		vita-elf-create  $< $@ db.json

$(NAME).elf: $(OBJDIR) $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

$(OBJDIR) :
		@[ -d $@ ] || mkdir -p $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/sound/%.c $(SRCDIR)/sound/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/input_hw/%.c $(SRCDIR)/input_hw/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/cart_hw/%.c $(SRCDIR)/cart_hw/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/cart_hw/svp/%.c
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/cart_hw/svp/%.c $(SRCDIR)/cart_hw/svp/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/cd_hw/%.c $(SRCDIR)/cd_hw/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/z80/%.c $(SRCDIR)/z80/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/m68k/%.c
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/ntsc/%.c $(SRCDIR)/ntsc/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/tremor/%.c $(SRCDIR)/tremor/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/tremor/%.c
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(OBJDIR)/%.o :	$(SRCDIR)/../psp2/%.c $(SRCDIR)/../psp2/%.h
		$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

clean:
	rm -f $(OBJECTS) $(NAME).velf $(NAME).elf
