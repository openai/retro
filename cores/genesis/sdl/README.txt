
DISCLAIMER:

THIS IS A VERY BASIC & UNSTABLE PORT WHICH ONLY PURPOSE
IS TO SHOW HOW TO USE GENESIS PLUS GX CORE & INTERFACE IT
WITH SDL IN PARTICULAR. BUILDS ARE NOT MEANT TO BE REALLY
USED FOR ANYTHING ELSE BUT EASIER CORE DEBUGGING ON WINDOWS.
PLEASE DO NOT DISTRIBUTE WIN32 BINARIES WITHOUT THIS NOTICE.
END USERS SHOULD PREFERABLY USE LIBRETRO PORT WITH RETROARCH.

----------------------------------------------------------------------------
 Genesis Plus (SDL Port) 
----------------------------------------------------------------------------

 based on the original version 1.3
 by Charles Mac Donald
 WWW: http://cgfm2.emuviews.com

 version 1.7.4
 backported from Genesis Plus GX
 by Eke-Eke
 WWW: http://code.google.com/p/genplus-gx
 E-mail: ekeeke31@gmail.com


 What's New
 ----------

 see CHANGELOG.txt


 Features 
 ---------

    * accurate emulation of SG-1000, Mark-III, Master System (I & II), Game Gear, Genesis / Mega Drive, Sega / Mega CD hardware models (incl. backwards compatibility modes)
    * NTSC (60Hz) & PAL (50Hz) video hardware emulation
    * accurate CDD, CDC & GFX chip emulation (Sega/Mega CD)
    * CD-DA fader emulation (Sega/Mega CD)
    * Mode 1 cartridge support (Sega/Mega CD)
    * highly accurate 68000 & Z80 CPU emulation
    * highly accurate VDP emulation (all rendering modes, mid-line changes, undocumented registers,…) & timings (HBLANK, DMA, FIFO, HV interrupts,…)
    * sample-accurate YM2612,YM2413, PSG, & PCM emulation (all sound chips are running at the original frequency)
    * cycle-accurate chip synchronization (68000’s/Z80/YM2612/PSG/PCM)
    * high-quality audio resampling using Blip Buffer
    * basic hardware latency emulation (VDP/68k, Z80/68k)
    * full overscan area emulation (horizontal & vertical color borders)
    * optional Game Gear extended screen mode
    * internal BOOT ROM support (Master System, Genesis / Mega Drive, Sega / Mega CD)
    * optional TMSS hardware emulation (Genesis / Mega Drive)
    * support for Blargg's software NTSC filters
    * preliminary PICO emulation
    * support for raw (.bin, .gen, .md, .sms, .gg & .sg) and interleaved (.smd & .mdx) ROM files
    * support for CUE+BIN, ISO+OGG & ISO+WAV CD image files 
    * 2-buttons, 3-buttons & 6-buttons controllers emulation
    * Sega Team Player & EA 4-Way Play multitaps emulation
    * Sega Mouse emulation
    * Sega Paddle Control & Sports Pad analog emulation
    * Terebi Oekaki tablet emulation
    * Sega Light Phaser, Menacer & Justifiers lightgun emulation
    * Sega Activator & XE-1AP analog controller emulation
    * SVP DSP (Virtua Racing) emulation
    * J-Cart adapter (Micro Machines & Pete Sampras series, Super Skidmarks) emulation
    * Backup RAM (max. 64KB), I2C (24Cxx), SPI (95xxx) & MicroWire (93C46) EEPROMs emulation
    * Sega/Mega CD RAM cart (max. 512KB) emulation
    * “official” ROM bankswitch hardware (Super Street Fighter 2) emulation
    * “official” backup RAM bankswitch hardware (Phantasy Star 4, Legend of Thor, Sonic the Hedgehog 3) emulation
    * support for all known unlicensed/pirate cartridges bankswitch & copy protection hardware
    * emulation of all known Master System & Game Gear cartridge “mappers” (incl. unlicensed Korean ones)
    * Game Genie & Action Replay hardware emulation
    * Sonic & Knuckles “Lock-On” hardware emulation
    * support for ROM image up to 10MB (Ultimate MK3 hack) 


 Usage
 -----

 The Windows version runs windowed in a 16-bit desktop with 48Hz sound using SDL but
 without joystick support.

 
 Controls
 -----

 Arrow Keys -   Directional pad
 A/Q,S,D,F  -   buttons A, B(1), C(2), START
 W,X,C,V    -   buttons X, Y, Z, MODE if 6-buttons controller is enabled
 Tab        -   Hard Reset 
 Esc        -   Exit program

 F2         -   Toggle Fullscreen/Windowed mode
 F4         -   Toggle Audio (Turbo mode must be disabled first)
 F6	    -   Toggle Turbo mode (Audio must be disabled first)
 F7	    -	Load Savestate (game.gpz)
 F8	    -	Save Savestate (game.gpz)
 F9	    -	Toggle VDP mode: PAL(50hz)/NTSC(60hz)
 F10	    -   Soft Reset
 F11	    -   Toggle Border emulation
 F12        -   Toggle Player # (test only)

 
 The mouse is used for lightguns, Sega Mouse, PICO & Terebi Oekaki tablet (automatically detected when loading supported game).

 A SRAM file (game.srm) is automatically saved on exit and loaded on startup.


 Credits
 --------

 Core(s) improvements & additional features by Eke-Eke

 Original code by Charles MacDonald

 Original Z80 core by Juergen Buchmueller

 Original Musashi 68k core by Karl Stenerud

 Original YM2612/YM2413 cores by Jarek Burczynski & Tatsuyuki Satoh

 Original SVP core by Notaz

 Blip Buffer & NTSC Video filter libraries by Shay Green (Blargg)

 3-Band EQ implementation by Neil C

 TREMOR VORBIS decoding library by Xiph.org

 Zlib by Jean-Loup Gailly & Mark Adler


 Aknowledgements
 ----------------

 The following emulator authors: Bart Trzynadlowski, Steve Snake, Stef, Notaz, AamirM

 The regular people at spritesmind.net and smspower.org.

 The MAME team for the CPU and sound chip emulators.

 Nemesis for the researches on the YM2612 and VDP.
 
 Tasco Deluxe for the documentation of Realtec mapper.

 Haze for the reverse-engineering of many unlicensed games protection.

