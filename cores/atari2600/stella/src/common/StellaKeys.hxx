//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: StellaKeys.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef STELLA_KEYS_HXX
#define STELLA_KEYS_HXX

/**
  This class implements a thin wrapper around the SDL keysym enumerations,
  such that SDL-specific code doesn't have to go into the internal parts of
  the codebase.  The keycodes are exactly the same, but from the POV of the
  rest of the code, they are *KBD* (keyboard) keys, not *SDL* keys.

  Once the codebase is ported to SDL2, the intent is to simply change this
  file without having to modify all other classes that use StellaKey.

  @author  Stephen Anthony
*/

// This comes directly from SDL_keysym.h
typedef enum {
        /** @name ASCII mapped keysyms
         *  The keyboard syms have been cleverly chosen to map to ASCII
         */
        /*@{*/
	KBDK_UNKNOWN		= 0,
	KBDK_FIRST		= 0,
	KBDK_BACKSPACE		= 8,
	KBDK_TAB		= 9,
	KBDK_CLEAR		= 12,
	KBDK_RETURN		= 13,
	KBDK_PAUSE		= 19,
	KBDK_ESCAPE		= 27,
	KBDK_SPACE		= 32,
	KBDK_EXCLAIM		= 33,
	KBDK_QUOTEDBL		= 34,
	KBDK_HASH		= 35,
	KBDK_DOLLAR		= 36,
	KBDK_AMPERSAND		= 38,
	KBDK_QUOTE		= 39,
	KBDK_LEFTPAREN		= 40,
	KBDK_RIGHTPAREN		= 41,
	KBDK_ASTERISK		= 42,
	KBDK_PLUS		= 43,
	KBDK_COMMA		= 44,
	KBDK_MINUS		= 45,
	KBDK_PERIOD		= 46,
	KBDK_SLASH		= 47,
	KBDK_0			= 48,
	KBDK_1			= 49,
	KBDK_2			= 50,
	KBDK_3			= 51,
	KBDK_4			= 52,
	KBDK_5			= 53,
	KBDK_6			= 54,
	KBDK_7			= 55,
	KBDK_8			= 56,
	KBDK_9			= 57,
	KBDK_COLON		= 58,
	KBDK_SEMICOLON		= 59,
	KBDK_LESS		= 60,
	KBDK_EQUALS		= 61,
	KBDK_GREATER		= 62,
	KBDK_QUESTION		= 63,
	KBDK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	KBDK_LEFTBRACKET	= 91,
	KBDK_BACKSLASH		= 92,
	KBDK_RIGHTBRACKET	= 93,
	KBDK_CARET		= 94,
	KBDK_UNDERSCORE		= 95,
	KBDK_BACKQUOTE		= 96,
	KBDK_a			= 97,
	KBDK_b			= 98,
	KBDK_c			= 99,
	KBDK_d			= 100,
	KBDK_e			= 101,
	KBDK_f			= 102,
	KBDK_g			= 103,
	KBDK_h			= 104,
	KBDK_i			= 105,
	KBDK_j			= 106,
	KBDK_k			= 107,
	KBDK_l			= 108,
	KBDK_m			= 109,
	KBDK_n			= 110,
	KBDK_o			= 111,
	KBDK_p			= 112,
	KBDK_q			= 113,
	KBDK_r			= 114,
	KBDK_s			= 115,
	KBDK_t			= 116,
	KBDK_u			= 117,
	KBDK_v			= 118,
	KBDK_w			= 119,
	KBDK_x			= 120,
	KBDK_y			= 121,
	KBDK_z			= 122,
	KBDK_DELETE		= 127,
	/* End of ASCII mapped keysyms */
        /*@}*/

	/** @name International keyboard syms */
        /*@{*/
	KBDK_WORLD_0		= 160,		/* 0xA0 */
	KBDK_WORLD_1		= 161,
	KBDK_WORLD_2		= 162,
	KBDK_WORLD_3		= 163,
	KBDK_WORLD_4		= 164,
	KBDK_WORLD_5		= 165,
	KBDK_WORLD_6		= 166,
	KBDK_WORLD_7		= 167,
	KBDK_WORLD_8		= 168,
	KBDK_WORLD_9		= 169,
	KBDK_WORLD_10		= 170,
	KBDK_WORLD_11		= 171,
	KBDK_WORLD_12		= 172,
	KBDK_WORLD_13		= 173,
	KBDK_WORLD_14		= 174,
	KBDK_WORLD_15		= 175,
	KBDK_WORLD_16		= 176,
	KBDK_WORLD_17		= 177,
	KBDK_WORLD_18		= 178,
	KBDK_WORLD_19		= 179,
	KBDK_WORLD_20		= 180,
	KBDK_WORLD_21		= 181,
	KBDK_WORLD_22		= 182,
	KBDK_WORLD_23		= 183,
	KBDK_WORLD_24		= 184,
	KBDK_WORLD_25		= 185,
	KBDK_WORLD_26		= 186,
	KBDK_WORLD_27		= 187,
	KBDK_WORLD_28		= 188,
	KBDK_WORLD_29		= 189,
	KBDK_WORLD_30		= 190,
	KBDK_WORLD_31		= 191,
	KBDK_WORLD_32		= 192,
	KBDK_WORLD_33		= 193,
	KBDK_WORLD_34		= 194,
	KBDK_WORLD_35		= 195,
	KBDK_WORLD_36		= 196,
	KBDK_WORLD_37		= 197,
	KBDK_WORLD_38		= 198,
	KBDK_WORLD_39		= 199,
	KBDK_WORLD_40		= 200,
	KBDK_WORLD_41		= 201,
	KBDK_WORLD_42		= 202,
	KBDK_WORLD_43		= 203,
	KBDK_WORLD_44		= 204,
	KBDK_WORLD_45		= 205,
	KBDK_WORLD_46		= 206,
	KBDK_WORLD_47		= 207,
	KBDK_WORLD_48		= 208,
	KBDK_WORLD_49		= 209,
	KBDK_WORLD_50		= 210,
	KBDK_WORLD_51		= 211,
	KBDK_WORLD_52		= 212,
	KBDK_WORLD_53		= 213,
	KBDK_WORLD_54		= 214,
	KBDK_WORLD_55		= 215,
	KBDK_WORLD_56		= 216,
	KBDK_WORLD_57		= 217,
	KBDK_WORLD_58		= 218,
	KBDK_WORLD_59		= 219,
	KBDK_WORLD_60		= 220,
	KBDK_WORLD_61		= 221,
	KBDK_WORLD_62		= 222,
	KBDK_WORLD_63		= 223,
	KBDK_WORLD_64		= 224,
	KBDK_WORLD_65		= 225,
	KBDK_WORLD_66		= 226,
	KBDK_WORLD_67		= 227,
	KBDK_WORLD_68		= 228,
	KBDK_WORLD_69		= 229,
	KBDK_WORLD_70		= 230,
	KBDK_WORLD_71		= 231,
	KBDK_WORLD_72		= 232,
	KBDK_WORLD_73		= 233,
	KBDK_WORLD_74		= 234,
	KBDK_WORLD_75		= 235,
	KBDK_WORLD_76		= 236,
	KBDK_WORLD_77		= 237,
	KBDK_WORLD_78		= 238,
	KBDK_WORLD_79		= 239,
	KBDK_WORLD_80		= 240,
	KBDK_WORLD_81		= 241,
	KBDK_WORLD_82		= 242,
	KBDK_WORLD_83		= 243,
	KBDK_WORLD_84		= 244,
	KBDK_WORLD_85		= 245,
	KBDK_WORLD_86		= 246,
	KBDK_WORLD_87		= 247,
	KBDK_WORLD_88		= 248,
	KBDK_WORLD_89		= 249,
	KBDK_WORLD_90		= 250,
	KBDK_WORLD_91		= 251,
	KBDK_WORLD_92		= 252,
	KBDK_WORLD_93		= 253,
	KBDK_WORLD_94		= 254,
	KBDK_WORLD_95		= 255,		/* 0xFF */
        /*@}*/

	/** @name Numeric keypad */
        /*@{*/
	KBDK_KP0		= 256,
	KBDK_KP1		= 257,
	KBDK_KP2		= 258,
	KBDK_KP3		= 259,
	KBDK_KP4		= 260,
	KBDK_KP5		= 261,
	KBDK_KP6		= 262,
	KBDK_KP7		= 263,
	KBDK_KP8		= 264,
	KBDK_KP9		= 265,
	KBDK_KP_PERIOD		= 266,
	KBDK_KP_DIVIDE		= 267,
	KBDK_KP_MULTIPLY	= 268,
	KBDK_KP_MINUS		= 269,
	KBDK_KP_PLUS		= 270,
	KBDK_KP_ENTER		= 271,
	KBDK_KP_EQUALS		= 272,
        /*@}*/

	/** @name Arrows + Home/End pad */
        /*@{*/
	KBDK_UP			= 273,
	KBDK_DOWN		= 274,
	KBDK_RIGHT		= 275,
	KBDK_LEFT		= 276,
	KBDK_INSERT		= 277,
	KBDK_HOME		= 278,
	KBDK_END		= 279,
	KBDK_PAGEUP		= 280,
	KBDK_PAGEDOWN		= 281,
        /*@}*/

	/** @name Function keys */
        /*@{*/
	KBDK_F1			= 282,
	KBDK_F2			= 283,
	KBDK_F3			= 284,
	KBDK_F4			= 285,
	KBDK_F5			= 286,
	KBDK_F6			= 287,
	KBDK_F7			= 288,
	KBDK_F8			= 289,
	KBDK_F9			= 290,
	KBDK_F10		= 291,
	KBDK_F11		= 292,
	KBDK_F12		= 293,
	KBDK_F13		= 294,
	KBDK_F14		= 295,
	KBDK_F15		= 296,
        /*@}*/

	/** @name Key state modifier keys */
        /*@{*/
	KBDK_NUMLOCK		= 300,
	KBDK_CAPSLOCK		= 301,
	KBDK_SCROLLOCK		= 302,
	KBDK_RSHIFT		= 303,
	KBDK_LSHIFT		= 304,
	KBDK_RCTRL		= 305,
	KBDK_LCTRL		= 306,
	KBDK_RALT		= 307,
	KBDK_LALT		= 308,
	KBDK_RMETA		= 309,
	KBDK_LMETA		= 310,
	KBDK_LSUPER		= 311,		/**< Left "Windows" key */
	KBDK_RSUPER		= 312,		/**< Right "Windows" key */
	KBDK_MODE		= 313,		/**< "Alt Gr" key */
	KBDK_COMPOSE		= 314,		/**< Multi-key compose key */
        /*@}*/

	/** @name Miscellaneous function keys */
        /*@{*/
	KBDK_HELP		= 315,
	KBDK_PRINT		= 316,
	KBDK_SYSREQ		= 317,
	KBDK_BREAK		= 318,
	KBDK_MENU		= 319,
	KBDK_POWER		= 320,		/**< Power Macintosh power key */
	KBDK_EURO		= 321,		/**< Some european keyboards */
	KBDK_UNDO		= 322,		/**< Atari keyboard has Undo */
        /*@}*/

	/* Add any other keys here */

	KBDK_LAST
} StellaKey;

// Just pass SDLMod directly as int (placeholder for now)
// The underlying code doesn't need to know how it's implemented
typedef int StellaMod;

#endif /* StellaKeys */
