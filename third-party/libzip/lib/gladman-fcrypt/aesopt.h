/*
 ---------------------------------------------------------------------------
 Copyright (c) 2002, Dr Brian Gladman <                 >, Worcester, UK.
 All rights reserved.

 LICENSE TERMS

 The free distribution and use of this software in both source and binary
 form is allowed (with or without changes) provided that:

   1. distributions of this source code include the above copyright
      notice, this list of conditions and the following disclaimer;

   2. distributions in binary form include the above copyright
      notice, this list of conditions and the following disclaimer
      in the documentation and/or other associated materials;

   3. the copyright holder's name is not used to endorse products
      built using this software without specific written permission.

 ALTERNATIVELY, provided that this notice is retained in full, this product
 may be distributed under the terms of the GNU General Public License (GPL),
 in which case the provisions of the GPL apply INSTEAD OF those given above.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue Date: 24/01/2003

 This file contains the compilation options for AES (Rijndael) and code
 that is common across encryption, key scheduling and table generation.

    OPERATION

    These source code files implement the AES algorithm Rijndael designed by
    Joan Daemen and Vincent Rijmen. The version in aes.c is designed for
    block and key sizes of 128, 192 and 256 bits (16, 24 and 32 bytes) while
    that in aespp.c provides for block and keys sizes of 128, 160, 192, 224
    and 256 bits (16, 20, 24, 28 and 32 bytes).  This file is a common header
    file for these two implementations and for aesref.c, which is a reference
    implementation.

    This version is designed for flexibility and speed using operations on
    32-bit words rather than operations on bytes.  It provides aes_both fixed
    and  dynamic block and key lengths and can also run with either big or
    little endian internal byte order (see aes.h).  It inputs block and key
    lengths in bytes with the legal values being  16, 24 and 32 for aes.c and
    16, 20, 24, 28 and 32 for aespp.c

    THE CIPHER INTERFACE

    aes_08t         (an unsigned  8-bit type)
    aes_32t         (an unsigned 32-bit type)
    aes_fret        (a signed 16 bit type for function return values)
    aes_good        (value != 0, a good return)
    aes_bad         (value == 0, an error return)
    struct aes_ctx  (structure for the cipher encryption context)
    struct aes_ctx  (structure for the cipher decryption context)
    aes_rval        the function return type (aes_fret if not DLL)

    C subroutine calls:

      aes_rval aes_set_block_size(unsigned int blen, aes_ctx cx[1]);
      aes_rval aes_set_encrypt_key(const unsigned char in_key[],
                                            unsigned int klen, aes_ctx cx[1]);
      aes_rval aes_encrypt_block(const unsigned char in_blk[],
                                unsigned char out_blk[], const aes_ctx cx[1]);

      aes_rval aes_dec_len(unsigned int blen, aes_ctx cx[1]);
      aes_rval aes_set_decrypt_key(const unsigned char in_key[],
                                            unsigned int klen, aes_ctx cx[1]);
      aes_rval aes_decrypt_block(const unsigned char in_blk[],
                                unsigned char out_blk[], const aes_ctx cx[1]);

    IMPORTANT NOTE: If you are using this C interface and your compiler does
    not set the memory used for objects to zero before use, you will need to
    ensure that cx.n_blk is set to zero before using these subroutine calls.

    C++ aes class subroutines:

      class AESclass    for encryption
      class AESclass    for decryption

      aes_rval len(unsigned int blen = 16);
      aes_rval key(const unsigned char in_key[], unsigned int klen);
      aes_rval blk(const unsigned char in_blk[], unsigned char out_blk[]);

      aes_rval len(unsigned int blen = 16);
      aes_rval key(const unsigned char in_key[], unsigned int klen);
      aes_rval blk(const unsigned char in_blk[], unsigned char out_blk[]);

    The block length inputs to set_block and set_key are in numbers of
    BYTES, not bits.  The calls to subroutines must be made in the above
    order but multiple calls can be made without repeating earlier calls
    if their parameters have not changed. If the cipher block length is
    variable but set_blk has not been called before cipher operations a
    value of 16 is assumed (that is, the AES block size). In contrast to
    earlier versions the block and key length parameters are now checked
    for correctness and the encryption and decryption routines check to
    ensure that an appropriate key has been set before they are called.

    COMPILATION

    The files used to provide AES (Rijndael) are

    a. aes.h for the definitions needed for use in C.
    b. aescpp.h for the definitions needed for use in C++.
    c. aesopt.h for setting compilation options (also includes common
       code).
    d. aescrypt.c for encryption and decrytpion, or
    e. aescrypt.asm for encryption and decryption using assembler code.
    f. aeskey.c for key scheduling.
    g. aestab.c for table loading or generation.

    The assembler code uses the NASM assembler. The above files provice
    block and key lengths of 16, 24 and 32 bytes (128, 192 and 256 bits).
    If aescrypp.c and aeskeypp.c are used instead of aescrypt.c and
    aeskey.c respectively, the block and key lengths can then be 16, 20,
    24, 28 or 32 bytes. However this code has not been optimised to the
    same extent and is hence slower (esepcially for the AES block size
    of 16 bytes).

    To compile AES (Rijndael) for use in C code use aes.h and exclude
    the AES_DLL define in aes.h

    To compile AES (Rijndael) for use in in C++ code use aescpp.h and
    exclude the AES_DLL define in aes.h

    To compile AES (Rijndael) in C as a Dynamic Link Library DLL) use
    aes.h, include the AES_DLL define and compile the DLL.  If using
    the test files to test the DLL, exclude aes.c from the test build
    project and compile it with the same defines as used for the DLL
    (ensure that the DLL path is correct)

    CONFIGURATION OPTIONS (here and in aes.h)

    a. define BLOCK_SIZE in aes.h to set the cipher block size (16, 24
       or 32 for the standard code, or 16, 20, 24, 28 or 32 for the
       extended code) or leave this undefined for dynamically variable
       block size (this will result in much slower code).
    b. set AES_DLL in aes.h if AES (Rijndael) is to be compiled as a DLL
    c. You may need to set PLATFORM_BYTE_ORDER to define the byte order.
    d. If you want the code to run in a specific internal byte order, then
       INTERNAL_BYTE_ORDER must be set accordingly.
    e. set other configuration options decribed below.
*/

#ifndef _AESOPT_H
#define _AESOPT_H

#if defined(__cplusplus)
extern "C"
{
#if 0
}
#endif
#endif

/*  START OF CONFIGURATION OPTIONS

    USE OF DEFINES

    Later in this section there are a number of defines that control the
    operation of the code.  In each section, the purpose of each define is
    explained so that the relevant form can be included or excluded by
    setting either 1's or 0's respectively on the branches of the related
    #if clauses.
*/

/*  DO NOT CHANGE THE FOLLOWING EIGHT DEFINES   */

#define NO_TABLES              0
#define ONE_TABLE              1
#define FOUR_TABLES            4
#define NONE                   0
#define PARTIAL                1
#define FULL                   2
#define AES_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */
#define AES_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */

/*  1. PLATFORM SPECIFIC INCLUDES */

#include "aes.h"

#include "config.h"

#if defined(WORDS_BIGENDIAN)
#  define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#else
#  define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#endif

/*  2. BYTE ORDER IN 32-BIT WORDS

    To obtain the highest speed on processors with 32-bit words, this code
    needs to determine the order in which bytes are packed into such words.
    The following block of code is an attempt to capture the most obvious
    ways in which various environemnts define byte order. It may well fail,
    in which case the definitions will need to be set by editing at the
    points marked **** EDIT HERE IF NECESSARY **** below.
*/
#if !defined(PLATFORM_BYTE_ORDER)
#if defined(LITTLE_ENDIAN) || defined(BIG_ENDIAN)
#  if defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    if defined(BYTE_ORDER)
#      if   (BYTE_ORDER == LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#      elif (BYTE_ORDER == BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#      endif
#    endif
#  elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#  elif !defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#  endif
#elif defined(_LITTLE_ENDIAN) || defined(_BIG_ENDIAN)
#  if defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    if defined(_BYTE_ORDER)
#      if   (_BYTE_ORDER == _LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#      elif (_BYTE_ORDER == _BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#      endif
#    endif
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#  elif !defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#  endif
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#elif (('1234' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER AES_LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER AES_BIG_ENDIAN
#endif
#endif

#if !defined(PLATFORM_BYTE_ORDER)
#  error Please set undetermined byte order (lines 241 or 243 of aesopt.h).
#endif

/*  3. FUNCTIONS REQUIRED

    This implementation provides five main subroutines which provide for
    setting block length, setting encryption and decryption keys and for
    encryption and decryption. When the assembler code is not being used
    the following definition blocks allow the selection of the routines
    that are to be included in the compilation.
*/
#if 1
#define ENCRYPTION
#define ENCRYPTION_KEY_SCHEDULE
#endif

#if 0
#define DECRYPTION
#define DECRYPTION_KEY_SCHEDULE
#endif

/*  4. ASSEMBLER SUPPORT

    This define (which can be on the command line) enables the use of the
    assembler code routines for encryption and decryption with the C code
    only providing key scheduling
*/
#if 0
#define AES_ASM
#endif

/*  5. BYTE ORDER WITHIN 32 BIT WORDS

    The fundamental data processing units in Rijndael are 8-bit bytes. The
    input, output and key input are all enumerated arrays of bytes in which
    bytes are numbered starting at zero and increasing to one less than the
    number of bytes in the array in question. This enumeration is only used
    for naming bytes and does not imply any adjacency or order relationship
    from one byte to another. When these inputs and outputs are considered
    as bit sequences, bits 8*n to 8*n+7 of the bit sequence are mapped to
    byte[n] with bit 8n+i in the sequence mapped to bit 7-i within the byte.
    In this implementation bits are numbered from 0 to 7 starting at the
    numerically least significant end of each byte (bit n represents 2^n).

    However, Rijndael can be implemented more efficiently using 32-bit
    words by packing bytes into words so that bytes 4*n to 4*n+3 are placed
    into word[n]. While in principle these bytes can be assembled into words
    in any positions, this implementation only supports the two formats in
    which bytes in adjacent positions within words also have adjacent byte
    numbers. This order is called big-endian if the lowest numbered bytes
    in words have the highest numeric significance and little-endian if the
    opposite applies.

    This code can work in either order irrespective of the order used by the
    machine on which it runs. Normally the internal byte order will be set
    to the order of the processor on which the code is to be run but this
    define can be used to reverse this in special situations
*/
#if 1
#define INTERNAL_BYTE_ORDER PLATFORM_BYTE_ORDER
#elif 0
#define INTERNAL_BYTE_ORDER AES_LITTLE_ENDIAN
#elif 0
#define INTERNAL_BYTE_ORDER AES_BIG_ENDIAN
#else
#error The internal byte order is not defined
#endif

/*  6. FAST INPUT/OUTPUT OPERATIONS.

    On some machines it is possible to improve speed by transferring the
    bytes in the input and output arrays to and from the internal 32-bit
    variables by addressing these arrays as if they are arrays of 32-bit
    words.  On some machines this will always be possible but there may
    be a large performance penalty if the byte arrays are not aligned on
    the normal word boundaries. On other machines this technique will
    lead to memory access errors when such 32-bit word accesses are not
    properly aligned. The option SAFE_IO avoids such problems but will
    often be slower on those machines that support misaligned access
    (especially so if care is taken to align the input  and output byte
    arrays on 32-bit word boundaries). If SAFE_IO is not defined it is
    assumed that access to byte arrays as if they are arrays of 32-bit
    words will not cause problems when such accesses are misaligned.
*/
#if 1 && !defined(_MSC_VER)
#define SAFE_IO
#endif

/*  7. LOOP UNROLLING

    The code for encryption and decrytpion cycles through a number of rounds
    that can be implemented either in a loop or by expanding the code into a
    long sequence of instructions, the latter producing a larger program but
    one that will often be much faster. The latter is called loop unrolling.
    There are also potential speed advantages in expanding two iterations in
    a loop with half the number of iterations, which is called partial loop
    unrolling.  The following options allow partial or full loop unrolling
    to be set independently for encryption and decryption
*/
#if 1
#define ENC_UNROLL  FULL
#elif 0
#define ENC_UNROLL  PARTIAL
#else
#define ENC_UNROLL  NONE
#endif

#if 1
#define DEC_UNROLL  FULL
#elif 0
#define DEC_UNROLL  PARTIAL
#else
#define DEC_UNROLL  NONE
#endif

/*  8. FAST FINITE FIELD OPERATIONS

    If this section is included, tables are used to provide faster finite
    field arithmetic (this has no effect if FIXED_TABLES is defined).
*/
#if 1
#define FF_TABLES
#endif

/*  9. INTERNAL STATE VARIABLE FORMAT

    The internal state of Rijndael is stored in a number of local 32-bit
    word varaibles which can be defined either as an array or as individual
    names variables. Include this section if you want to store these local
    varaibles in arrays. Otherwise individual local variables will be used.
*/
#if 1
#define ARRAYS
#endif

/* In this implementation the columns of the state array are each held in
   32-bit words. The state array can be held in various ways: in an array
   of words, in a number of individual word variables or in a number of
   processor registers. The following define maps a variable name x and
   a column number c to the way the state array variable is to be held.
   The first define below maps the state into an array x[c] whereas the
   second form maps the state into a number of individual variables x0,
   x1, etc.  Another form could map individual state colums to machine
   register names.
*/

#if defined(ARRAYS)
#define s(x,c) x[c]
#else
#define s(x,c) x##c
#endif

/*  10. VARIABLE BLOCK SIZE SPEED

    This section is only relevant if you wish to use the variable block
    length feature of the code.  Include this section if you place more
    emphasis on speed rather than code size.
*/
#if 1
#define FAST_VARIABLE
#endif

/*  11. FIXED OR DYNAMIC TABLES

    When this section is included the tables used by the code are comipled
    statically into the binary file.  Otherwise they are computed once when
    the code is first used.
*/
#if 1
#define FIXED_TABLES
#endif

/*  12. GLOBAL VARIABLES

    In some circumstances global variables to hold the fixed tables are not
    possible so the tables have to be placed in a structure that is passed
    around at run time through the AES API calls. In this case FIXED_TABLES
    cannot be used. GLOBALS must be defined in order to use the assembler
    version of this code. If GLOBALS is not defined, then AES_TABLE_PTR must
    be defined either on the command line or early in aes.h in order to
    provide space for the table pointer in the AES context.
*/
#if 1 || defined(AES_ASM)
#define GLOBALS
#elif !defined(AES_TABLE_PTR)
#error no context space for table pointer (compile with AES_TABLE_PTR defined)
#elif defined(FIXED_TABLES)
#undef FIXED_TABLES
#endif

/*  13. INTERNAL TABLE CONFIGURATION

    This cipher proceeds by repeating in a number of cycles known as 'rounds'
    which are implemented by a round function which can optionally be speeded
    up using tables.  The basic tables are each 256 32-bit words, with either
    one or four tables being required for each round function depending on
    how much speed is required. The encryption and decryption round functions
    are different and the last encryption and decrytpion round functions are
    different again making four different round functions in all.

    This means that:
      1. Normal encryption and decryption rounds can each use either 0, 1
         or 4 tables and table spaces of 0, 1024 or 4096 bytes each.
      2. The last encryption and decryption rounds can also use either 0, 1
         or 4 tables and table spaces of 0, 1024 or 4096 bytes each.

    Include or exclude the appropriate definitions below to set the number
    of tables used by this implementation.
*/

#if 1   /* set tables for the normal encryption round */
#define ENC_ROUND   FOUR_TABLES
#elif 0
#define ENC_ROUND   ONE_TABLE
#else
#define ENC_ROUND   NO_TABLES
#endif

#if 1   /* set tables for the last encryption round */
#define LAST_ENC_ROUND  FOUR_TABLES
#elif 0
#define LAST_ENC_ROUND  ONE_TABLE
#else
#define LAST_ENC_ROUND  NO_TABLES
#endif

#if 1   /* set tables for the normal decryption round */
#define DEC_ROUND   FOUR_TABLES
#elif 0
#define DEC_ROUND   ONE_TABLE
#else
#define DEC_ROUND   NO_TABLES
#endif

#if 1   /* set tables for the last decryption round */
#define LAST_DEC_ROUND  FOUR_TABLES
#elif 0
#define LAST_DEC_ROUND  ONE_TABLE
#else
#define LAST_DEC_ROUND  NO_TABLES
#endif

/*  The decryption key schedule can be speeded up with tables in the same
    way that the round functions can.  Include or exclude the following
    defines to set this requirement.
*/
#if 1
#define KEY_SCHED   FOUR_TABLES
#elif 0
#define KEY_SCHED   ONE_TABLE
#else
#define KEY_SCHED   NO_TABLES
#endif

/* END OF CONFIGURATION OPTIONS */

#if defined(BLOCK_SIZE) && ((BLOCK_SIZE & 3) || BLOCK_SIZE < 16 || BLOCK_SIZE > 32)
#error An illegal block size has been specified.
#endif

#if !defined(BLOCK_SIZE)
#define RC_LENGTH   29
#else
#define RC_LENGTH   (5 * BLOCK_SIZE / 4 - (BLOCK_SIZE == 16 ? 10 : 11))
#endif

/* Disable at least some poor combinations of options */

#if ENC_ROUND == NO_TABLES && LAST_ENC_ROUND != NO_TABLES
#undef  LAST_ENC_ROUND
#define LAST_ENC_ROUND  NO_TABLES
#elif ENC_ROUND == ONE_TABLE && LAST_ENC_ROUND == FOUR_TABLES
#undef  LAST_ENC_ROUND
#define LAST_ENC_ROUND  ONE_TABLE
#endif

#if ENC_ROUND == NO_TABLES && ENC_UNROLL != NONE
#undef  ENC_UNROLL
#define ENC_UNROLL  NONE
#endif

#if DEC_ROUND == NO_TABLES && LAST_DEC_ROUND != NO_TABLES
#undef  LAST_DEC_ROUND
#define LAST_DEC_ROUND  NO_TABLES
#elif DEC_ROUND == ONE_TABLE && LAST_DEC_ROUND == FOUR_TABLES
#undef  LAST_DEC_ROUND
#define LAST_DEC_ROUND  ONE_TABLE
#endif

#if DEC_ROUND == NO_TABLES && DEC_UNROLL != NONE
#undef  DEC_UNROLL
#define DEC_UNROLL  NONE
#endif

/*  upr(x,n):  rotates bytes within words by n positions, moving bytes to
               higher index positions with wrap around into low positions
    ups(x,n):  moves bytes by n positions to higher index positions in
               words but without wrap around
    bval(x,n): extracts a byte from a word

    NOTE:      The definitions given here are intended only for use with
               unsigned variables and with shift counts that are compile
               time constants
*/

#if (INTERNAL_BYTE_ORDER == AES_LITTLE_ENDIAN)
#define upr(x,n)        (((aes_32t)(x) << (8 * (n))) | ((aes_32t)(x) >> (32 - 8 * (n))))
#define ups(x,n)        ((aes_32t) (x) << (8 * (n)))
#define bval(x,n)       ((aes_08t)((x) >> (8 * (n))))
#define bytes2word(b0, b1, b2, b3)  \
        (((aes_32t)(b3) << 24) | ((aes_32t)(b2) << 16) | ((aes_32t)(b1) << 8) | (b0))
#endif

#if (INTERNAL_BYTE_ORDER == AES_BIG_ENDIAN)
#define upr(x,n)        (((aes_32t)(x) >> (8 * (n))) | ((aes_32t)(x) << (32 - 8 * (n))))
#define ups(x,n)        ((aes_32t) (x) >> (8 * (n))))
#define bval(x,n)       ((aes_08t)((x) >> (24 - 8 * (n))))
#define bytes2word(b0, b1, b2, b3)  \
        (((aes_32t)(b0) << 24) | ((aes_32t)(b1) << 16) | ((aes_32t)(b2) << 8) | (b3))
#endif

#if defined(SAFE_IO)

#define word_in(x)      bytes2word((x)[0], (x)[1], (x)[2], (x)[3])
#define word_out(x,v)   { (x)[0] = bval(v,0); (x)[1] = bval(v,1);   \
                          (x)[2] = bval(v,2); (x)[3] = bval(v,3);   }

#elif (INTERNAL_BYTE_ORDER == PLATFORM_BYTE_ORDER)

#define word_in(x)      (*(aes_32t*)(x))
#define word_out(x,v)   (*(aes_32t*)(x) = (v))

#else

#if !defined(bswap_32)
#define brot(x,n)       (((aes_32t)(x) <<  n) | ((aes_32t)(x) >> (32 - n)))
#define bswap_32(x)     ((brot((x),8) & 0x00ff00ff) | (brot((x),24) & 0xff00ff00))
#endif

#define word_in(x)      bswap_32(*(aes_32t*)(x))
#define word_out(x,v)   (*(aes_32t*)(x) = bswap_32(v))

#endif

/* the finite field modular polynomial and elements */

#define WPOLY   0x011b
#define BPOLY     0x1b

/* multiply four bytes in GF(2^8) by 'x' {02} in parallel */

#define m1  0x80808080
#define m2  0x7f7f7f7f
#define FFmulX(x)  ((((x) & m2) << 1) ^ ((((x) & m1) >> 7) * BPOLY))

/* The following defines provide alternative definitions of FFmulX that might
   give improved performance if a fast 32-bit multiply is not available. Note
   that a temporary variable u needs to be defined where FFmulX is used.

#define FFmulX(x) (u = (x) & m1, u |= (u >> 1), ((x) & m2) << 1) ^ ((u >> 3) | (u >> 6))
#define m4  (0x01010101 * BPOLY)
#define FFmulX(x) (u = (x) & m1, ((x) & m2) << 1) ^ ((u - (u >> 7)) & m4)
*/

/* Work out which tables are needed for the different options   */

#ifdef  AES_ASM
#ifdef  ENC_ROUND
#undef  ENC_ROUND
#endif
#define ENC_ROUND   FOUR_TABLES
#ifdef  LAST_ENC_ROUND
#undef  LAST_ENC_ROUND
#endif
#define LAST_ENC_ROUND  FOUR_TABLES
#ifdef  DEC_ROUND
#undef  DEC_ROUND
#endif
#define DEC_ROUND   FOUR_TABLES
#ifdef  LAST_DEC_ROUND
#undef  LAST_DEC_ROUND
#endif
#define LAST_DEC_ROUND  FOUR_TABLES
#ifdef  KEY_SCHED
#undef  KEY_SCHED
#define KEY_SCHED   FOUR_TABLES
#endif
#endif

#if defined(ENCRYPTION) || defined(AES_ASM)
#if ENC_ROUND == ONE_TABLE
#define FT1_SET
#elif ENC_ROUND == FOUR_TABLES
#define FT4_SET
#else
#define SBX_SET
#endif
#if LAST_ENC_ROUND == ONE_TABLE
#define FL1_SET
#elif LAST_ENC_ROUND == FOUR_TABLES
#define FL4_SET
#elif !defined(SBX_SET)
#define SBX_SET
#endif
#endif

#if defined(DECRYPTION) || defined(AES_ASM)
#if DEC_ROUND == ONE_TABLE
#define IT1_SET
#elif DEC_ROUND == FOUR_TABLES
#define IT4_SET
#else
#define ISB_SET
#endif
#if LAST_DEC_ROUND == ONE_TABLE
#define IL1_SET
#elif LAST_DEC_ROUND == FOUR_TABLES
#define IL4_SET
#elif !defined(ISB_SET)
#define ISB_SET
#endif
#endif

#if defined(ENCRYPTION_KEY_SCHEDULE) || defined(DECRYPTION_KEY_SCHEDULE)
#if KEY_SCHED == ONE_TABLE
#define LS1_SET
#define IM1_SET
#elif KEY_SCHED == FOUR_TABLES
#define LS4_SET
#define IM4_SET
#elif !defined(SBX_SET)
#define SBX_SET
#endif
#endif

/*  If there are no global variables, the AES tables are placed in
    a structure and a pointer is added to the AES context. If this
    facility is used, the calling program has to ensure that this
    pointer is managed appropriately. In particular, the value of
    the t_dec(in,it) item in the table structure must be set to zero
    in order to ensure that the tables are initialised. In practice
    the three code sequences in aeskey.c that control the calls to
    gen_tabs() and the gen_tabs() routine itself will require some
    changes for a specific implementation. If global variables are
    available it will generally be preferable to use them with the
    precomputed FIXED_TABLES option that uses static global tables.

    The following defines can be used to control the way the tables
    are defined, initialised and used in embedded environments that
    require special features for these purposes

    the 't_dec' construction is used to declare fixed table arrays
    the 't_set' construction is used to set fixed table values
    the 't_use' construction is used to access fixed table values

    256 byte tables:

        t_xxx(s,box)    => forward S box
        t_xxx(i,box)    => inverse S box

    256 32-bit word OR 4 x 256 32-bit word tables:

        t_xxx(f,n)      => forward normal round
        t_xxx(f,l)      => forward last round
        t_xxx(i,n)      => inverse normal round
        t_xxx(i,l)      => inverse last round
        t_xxx(l,s)      => key schedule table
        t_xxx(i,m)      => key schedule table

    Other variables and tables:

        t_xxx(in,it)    => the table initialsation flag
        t_xxx(r,c)      => the rcon table
*/

#define t_dec(m,n) t_##m##n
#define t_set(m,n) t_##m##n  /* may be redefined once below */
#define t_use(m,n) t_##m##n  /* may be redefined once below */

#ifdef  FIXED_TABLES

#define prefx    INTERNAL const
#elif defined(GLOBALS)
#define prefx    extern
extern aes_08t   t_dec(in,it);
void gen_tabs(void);
#else
#define prefx
void gen_tabs(aes_ctx cx[1]);

typedef struct {
#endif

prefx aes_32t  t_dec(r,c)[RC_LENGTH];

#ifdef  SBX_SET
prefx aes_08t t_dec(s,box)[256];
#endif

#ifdef  ISB_SET
prefx aes_08t t_dec(i,box)[256];
#endif

#ifdef  FT1_SET
prefx aes_32t t_dec(f,n)[256];
#endif

#ifdef  FT4_SET
prefx aes_32t t_dec(f,n)[4][256];
#endif

#ifdef  FL1_SET
prefx aes_32t t_dec(f,l)[256];
#endif

#ifdef  FL4_SET
prefx aes_32t t_dec(f,l)[4][256];
#endif

#ifdef  IT1_SET
prefx aes_32t t_dec(i,n)[256];
#endif

#ifdef  IT4_SET
prefx aes_32t t_dec(i,n)[4][256];
#endif

#ifdef  IL1_SET
prefx aes_32t t_dec(i,l)[256];
#endif

#ifdef  IL4_SET
prefx aes_32t t_dec(i,l)[4][256];
#endif

#ifdef  LS1_SET
#ifdef  FL1_SET
#undef  LS1_SET
#else
prefx aes_32t t_dec(l,s)[256];
#endif
#endif

#ifdef  LS4_SET
#ifdef  FL4_SET
#undef  LS4_SET
#else
prefx aes_32t t_dec(l,s)[4][256];
#endif
#endif

#ifdef  IM1_SET
prefx aes_32t t_dec(i,m)[256];
#endif

#ifdef  IM4_SET
prefx aes_32t t_dec(i,m)[4][256];
#endif

prefx aes_08t  t_dec(in,it);

#ifndef GLOBALS
} s_ty;

extern s_ty s_t;

/* modify the table set macro to cope with a static structure */
#undef  t_set
#define t_set(m,n) (s_t.t_##m##n)

/* modify the table use macro to cope with a context pointer  */
#undef  t_use
#define t_use(m,n) (((s_ty*)cx->t_ptr)->t_##m##n)

#endif

/* Set the number of columns in nc.  Note that it is important
   that nc is a constant which is known at compile time if the
   highest speed version of the code is needed.
*/

#if defined(BLOCK_SIZE)
#define nc  (BLOCK_SIZE >> 2)
#else
#define nc  (cx->n_blk >> 2)
#endif

/* generic definitions of Rijndael macros that use tables    */

#define no_table(x,box,vf,rf,c) bytes2word( \
    box[bval(vf(x,0,c),rf(0,c))], \
    box[bval(vf(x,1,c),rf(1,c))], \
    box[bval(vf(x,2,c),rf(2,c))], \
    box[bval(vf(x,3,c),rf(3,c))])

#define one_table(x,op,tab,vf,rf,c) \
 (     tab[bval(vf(x,0,c),rf(0,c))] \
  ^ op(tab[bval(vf(x,1,c),rf(1,c))],1) \
  ^ op(tab[bval(vf(x,2,c),rf(2,c))],2) \
  ^ op(tab[bval(vf(x,3,c),rf(3,c))],3))

#define four_tables(x,tab,vf,rf,c) \
 (  tab[0][bval(vf(x,0,c),rf(0,c))] \
  ^ tab[1][bval(vf(x,1,c),rf(1,c))] \
  ^ tab[2][bval(vf(x,2,c),rf(2,c))] \
  ^ tab[3][bval(vf(x,3,c),rf(3,c))])

#define vf1(x,r,c)  (x)
#define rf1(r,c)    (r)
#define rf2(r,c)    ((8+r-c)&3)

/* perform forward and inverse column mix operation on four bytes in long word x in */
/* parallel. NOTE: x must be a simple variable, NOT an expression in these macros.  */

#define dec_fmvars
#if defined(FM4_SET)    /* not currently used */
#define fwd_mcol(x)     four_tables(x,t_use(f,m),vf1,rf1,0)
#elif defined(FM1_SET)  /* not currently used */
#define fwd_mcol(x)     one_table(x,upr,t_use(f,m),vf1,rf1,0)
#else
#undef  dec_fmvars
#define dec_fmvars      aes_32t f1, f2;
#define fwd_mcol(x)     (f1 = (x), f2 = FFmulX(f1), f2 ^ upr(f1 ^ f2, 3) ^ upr(f1, 2) ^ upr(f1, 1))
#endif

#define dec_imvars
#if defined(IM4_SET)
#define inv_mcol(x)     four_tables(x,t_use(i,m),vf1,rf1,0)
#elif defined(IM1_SET)
#define inv_mcol(x)     one_table(x,upr,t_use(i,m),vf1,rf1,0)
#else
#undef  dec_imvars
#define dec_imvars      aes_32t    f2, f4, f8, f9;
#define inv_mcol(x) \
    (f9 = (x), f2 = FFmulX(f9), f4 = FFmulX(f2), f8 = FFmulX(f4), f9 ^= f8, \
    f2 ^= f4 ^ f8 ^ upr(f2 ^ f9,3) ^ upr(f4 ^ f9,2) ^ upr(f9,1))
#endif

#if defined(FL4_SET)
#define ls_box(x,c)     four_tables(x,t_use(f,l),vf1,rf2,c)
#elif   defined(LS4_SET)
#define ls_box(x,c)     four_tables(x,t_use(l,s),vf1,rf2,c)
#elif defined(FL1_SET)
#define ls_box(x,c)     one_table(x,upr,t_use(f,l),vf1,rf2,c)
#elif defined(LS1_SET)
#define ls_box(x,c)     one_table(x,upr,t_use(l,s),vf1,rf2,c)
#else
#define ls_box(x,c)     no_table(x,t_use(s,box),vf1,rf2,c)
#endif

#if defined(__cplusplus)
}
#endif

#endif
