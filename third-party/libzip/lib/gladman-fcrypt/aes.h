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

 This file contains the definitions required to use AES and Rijndael in C.
*/

#ifndef _AES_H
#define _AES_H

#if defined(__cplusplus)
extern "C"
{
#if 0
}
#endif
#endif

/* If a table pointer is needed in the AES context, include the define  */
/* #define AES_TABLE_PTR                                                */

#include "config.h"

typedef zip_uint8_t aes_08t;
typedef zip_uint32_t aes_32t;

/* This BLOCK_SIZE is in BYTES.  It can have the values 16, 24, 32 or   */
/* undefined for use with aescrypt.c and aeskey.c, or 16, 20, 24, 28,   */
/* 32 or undefined for use with aescrypp.c and aeskeypp.c.   When the   */
/* BLOCK_SIZE is left undefined a version that provides a dynamically   */
/* variable block size is produced but this is MUCH slower.             */

#define BLOCK_SIZE  16

/* key schedule length (in 32-bit words)                                */

#if !defined(BLOCK_SIZE)
#define KS_LENGTH   128
#else
#define KS_LENGTH   (4 * BLOCK_SIZE)
#endif

typedef unsigned int aes_fret;   /* type for function return value      */
#define aes_bad      0           /* bad function return value           */
#define aes_good     1           /* good function return value          */
#ifndef AES_DLL                  /* implement normal or DLL functions   */
#define aes_rval     aes_fret
#else
#define aes_rval     aes_fret __declspec(dllexport) _stdcall
#endif

typedef struct                     /* the AES context for encryption    */
{   aes_32t    k_sch[KS_LENGTH];   /* the encryption key schedule       */
    aes_32t    n_rnd;              /* the number of cipher rounds       */
    aes_32t    n_blk;              /* the number of bytes in the state  */
#if defined(AES_TABLE_PTR)         /* where global variables are not    */
    void      *t_ptr;              /* available this pointer is used    */
#endif                             /* to point to the fixed tables      */
} aes_ctx;

/* The block length (blen) is input in bytes when it is in the range    */
/* 16 <= blen <= 32 or in bits when in the range 128 <= blen <= 256     */
/* Only 16 bytes (128 bits) is legal for AES but the files aescrypt.c   */
/* and aeskey.c provide support for 16, 24 and 32 byte (128, 192 and    */
/* 256 bit) blocks while aescrypp.c and aeskeypp.c provide support for  */
/* 16, 20, 24, 28 and 32 byte (128, 160, 192, 224 and 256 bit) blocks.  */
/* The value aes_good is returned if the requested block size is legal, */
/* otherwise aes_bad is returned.                                       */

#if !defined(BLOCK_SIZE)
INTERNAL aes_rval aes_set_block_size(unsigned int blen, aes_ctx cx[1]);
#endif

/* The key length (klen) is input in bytes when it is in the range      */
/* 16 <= klen <= 32 or in bits when in the range 128 <= klen <= 256     */
/* The files aescrypt.c and aeskey.c provide support for 16, 24 and     */
/* 32 byte (128, 192 and 256 bit) keys while aescrypp.c and aeskeypp.c  */
/* provide support for 16, 20, 24, 28 and 32 byte (128, 160, 192, 224   */
/* and 256 bit) keys.  The value aes_good is returned if the requested  */
/* key size is legal, otherwise aes_bad is returned.                    */

#ifdef BUILDING_FCRYPT
INTERNAL aes_rval aes_set_encrypt_key(const unsigned char in_key[],
                                        unsigned int klen, aes_ctx cx[1]);
INTERNAL aes_rval aes_encrypt_block(const unsigned char in_blk[],
                            unsigned char out_blk[], const aes_ctx cx[1]);
#endif

#if 0 /* not used */
INTERNAL aes_rval aes_set_decrypt_key(const unsigned char in_key[],
                                        unsigned int klen, aes_ctx cx[1]);
INTERNAL aes_rval aes_decrypt_block(const unsigned char in_blk[],
                            unsigned char out_blk[], const aes_ctx cx[1]);
#endif

#if defined(__cplusplus)
}
#endif

#endif
