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

 This file contains the code for implementing the key schedule for AES and
 Rijndael for block and key sizes of 16, 24, and 32 bytes.
*/

#include "aesopt.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#if defined(BLOCK_SIZE) && (BLOCK_SIZE & 7)
#error An illegal block size has been specified.
#endif

/* Subroutine to set the block size (if variable). The value can be
   in bytes, with legal values of 16, 24 and 32, or in bits, with
   legal values of 128, 192 and 256.
*/

#if !defined(BLOCK_SIZE)

INTERNAL aes_rval aes_set_block_size(unsigned int blen, aes_ctx cx[1])
{
#if !defined(FIXED_TABLES)
#ifdef GLOBALS
    if(!t_use(in,it)) gen_tabs();
#else
    if(!cx->t_ptr || !t_use(in,it)) gen_tabs(cx);
#endif
#endif
    if(((blen & 7) || blen < 16 || blen > 32) && ((blen & 63) || blen < 128 || blen > 256))
    {
        cx->n_blk = 0; return aes_bad;
    }
    else
    {
        cx->n_blk = blen >> (blen < 128 ? 0 : 3); return aes_good;
    }
}

#endif

/* Initialise the key schedule from the user supplied key. The key
   length can be specified in bytes, with legal values of 16, 24
   and 32, or in bits, with legal values of 128, 192 and 256. These
   values correspond with Nk values of 4, 6 and 8 respectively.

   The following macros implement a single cycle in the key
   schedule generation process. The number of cycles needed
   for each cx->n_col and nk value is:

    nk =             4  5  6  7  8
    ------------------------------
    cx->n_col = 4   10  9  8  7  7
    cx->n_col = 5   14 11 10  9  9
    cx->n_col = 6   19 15 12 11 11
    cx->n_col = 7   21 19 16 13 14
    cx->n_col = 8   29 23 19 17 14
*/

#define ke4(k,i) \
{   k[4*(i)+4] = ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[4*(i)+5] = ss[1] ^= ss[0]; \
    k[4*(i)+6] = ss[2] ^= ss[1]; k[4*(i)+7] = ss[3] ^= ss[2]; \
}
#define kel4(k,i) \
{   k[4*(i)+4] = ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[4*(i)+5] = ss[1] ^= ss[0]; \
    k[4*(i)+6] = ss[2] ^= ss[1]; k[4*(i)+7] = ss[3] ^= ss[2]; \
}

#define ke6(k,i) \
{   k[6*(i)+ 6] = ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[6*(i)+ 7] = ss[1] ^= ss[0]; \
    k[6*(i)+ 8] = ss[2] ^= ss[1]; k[6*(i)+ 9] = ss[3] ^= ss[2]; \
    k[6*(i)+10] = ss[4] ^= ss[3]; k[6*(i)+11] = ss[5] ^= ss[4]; \
}
#define kel6(k,i) \
{   k[6*(i)+ 6] = ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[6*(i)+ 7] = ss[1] ^= ss[0]; \
    k[6*(i)+ 8] = ss[2] ^= ss[1]; k[6*(i)+ 9] = ss[3] ^= ss[2]; \
}

#define ke8(k,i) \
{   k[8*(i)+ 8] = ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[8*(i)+ 9] = ss[1] ^= ss[0]; \
    k[8*(i)+10] = ss[2] ^= ss[1]; k[8*(i)+11] = ss[3] ^= ss[2]; \
    k[8*(i)+12] = ss[4] ^= ls_box(ss[3],0); k[8*(i)+13] = ss[5] ^= ss[4]; \
    k[8*(i)+14] = ss[6] ^= ss[5]; k[8*(i)+15] = ss[7] ^= ss[6]; \
}
#define kel8(k,i) \
{   k[8*(i)+ 8] = ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[8*(i)+ 9] = ss[1] ^= ss[0]; \
    k[8*(i)+10] = ss[2] ^= ss[1]; k[8*(i)+11] = ss[3] ^= ss[2]; \
}

#if defined(ENCRYPTION_KEY_SCHEDULE)

INTERNAL aes_rval aes_set_encrypt_key(const unsigned char in_key[], unsigned int klen, aes_ctx cx[1])
{   aes_32t    ss[8];

#if !defined(FIXED_TABLES)
#ifdef GLOBALS
    if(!t_use(in,it)) gen_tabs();
#else
    if(!cx->t_ptr || !t_use(in,it)) gen_tabs(cx);
#endif
#endif

#if !defined(BLOCK_SIZE)
    if(!cx->n_blk) cx->n_blk = 16;
#else
    cx->n_blk = BLOCK_SIZE;
#endif

    if(((klen & 7) || klen < 16 || klen > 32) && ((klen & 63) || klen < 128 || klen > 256))
    {
        cx->n_rnd = 0; return aes_bad;
    }

    klen >>= (klen < 128 ? 2 : 5);
    cx->n_blk = (cx->n_blk & ~3U) | 1;

    cx->k_sch[0] = ss[0] = word_in(in_key     );
    cx->k_sch[1] = ss[1] = word_in(in_key +  4);
    cx->k_sch[2] = ss[2] = word_in(in_key +  8);
    cx->k_sch[3] = ss[3] = word_in(in_key + 12);

#if (BLOCK_SIZE == 16) && (ENC_UNROLL != NONE)

    switch(klen)
    {
    case 4:
        ke4(cx->k_sch, 0); ke4(cx->k_sch, 1);
        ke4(cx->k_sch, 2); ke4(cx->k_sch, 3);
        ke4(cx->k_sch, 4); ke4(cx->k_sch, 5);
        ke4(cx->k_sch, 6); ke4(cx->k_sch, 7);
        ke4(cx->k_sch, 8); kel4(cx->k_sch, 9);
        cx->n_rnd = 10; break;
    case 6:
        cx->k_sch[4] = ss[4] = word_in(in_key + 16);
        cx->k_sch[5] = ss[5] = word_in(in_key + 20);
        ke6(cx->k_sch, 0); ke6(cx->k_sch, 1);
        ke6(cx->k_sch, 2); ke6(cx->k_sch, 3);
        ke6(cx->k_sch, 4); ke6(cx->k_sch, 5);
        ke6(cx->k_sch, 6); kel6(cx->k_sch, 7);
        cx->n_rnd = 12; break;
    case 8:
        cx->k_sch[4] = ss[4] = word_in(in_key + 16);
        cx->k_sch[5] = ss[5] = word_in(in_key + 20);
        cx->k_sch[6] = ss[6] = word_in(in_key + 24);
        cx->k_sch[7] = ss[7] = word_in(in_key + 28);
        ke8(cx->k_sch, 0); ke8(cx->k_sch, 1);
        ke8(cx->k_sch, 2); ke8(cx->k_sch, 3);
        ke8(cx->k_sch, 4); ke8(cx->k_sch, 5);
        kel8(cx->k_sch, 6);
        cx->n_rnd = 14; break;
    default:
        ;
    }
#else
    cx->n_rnd = (klen > nc ? klen : nc) + 6;
    {   aes_32t i, l;
        l = (nc * cx->n_rnd + nc - 1) / klen;

        switch(klen)
        {
        case 4:
            for(i = 0; i < l; ++i)
                ke4(cx->k_sch, i);
            break;
        case 6:
            cx->k_sch[4] = ss[4] = word_in(in_key + 16);
            cx->k_sch[5] = ss[5] = word_in(in_key + 20);
            for(i = 0; i < l; ++i)
                ke6(cx->k_sch, i);
            break;
        case 8:
            cx->k_sch[4] = ss[4] = word_in(in_key + 16);
            cx->k_sch[5] = ss[5] = word_in(in_key + 20);
            cx->k_sch[6] = ss[6] = word_in(in_key + 24);
            cx->k_sch[7] = ss[7] = word_in(in_key + 28);
            for(i = 0; i < l; ++i)
                ke8(cx->k_sch,  i);
            break;
        default:
            ;
        }
    }
#endif

    return aes_good;
}

#endif

#if defined(DECRYPTION_KEY_SCHEDULE)

#if (DEC_ROUND != NO_TABLES)
#define d_vars  dec_imvars
#define ff(x)   inv_mcol(x)
#else
#define ff(x)   (x)
#define d_vars
#endif

#if 1
#define kdf4(k,i) \
{   ss[0] = ss[0] ^ ss[2] ^ ss[1] ^ ss[3]; ss[1] = ss[1] ^ ss[3]; ss[2] = ss[2] ^ ss[3]; ss[3] = ss[3]; \
    ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; ss[i % 4] ^= ss[4]; \
    ss[4] ^= k[4*(i)];   k[4*(i)+4] = ff(ss[4]); ss[4] ^= k[4*(i)+1]; k[4*(i)+5] = ff(ss[4]); \
    ss[4] ^= k[4*(i)+2]; k[4*(i)+6] = ff(ss[4]); ss[4] ^= k[4*(i)+3]; k[4*(i)+7] = ff(ss[4]); \
}
#define kd4(k,i) \
{   ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; ss[i % 4] ^= ss[4]; ss[4] = ff(ss[4]); \
    k[4*(i)+4] = ss[4] ^= k[4*(i)]; k[4*(i)+5] = ss[4] ^= k[4*(i)+1]; \
    k[4*(i)+6] = ss[4] ^= k[4*(i)+2]; k[4*(i)+7] = ss[4] ^= k[4*(i)+3]; \
}
#define kdl4(k,i) \
{   ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; ss[i % 4] ^= ss[4]; \
    k[4*(i)+4] = (ss[0] ^= ss[1]) ^ ss[2] ^ ss[3]; k[4*(i)+5] = ss[1] ^ ss[3]; \
    k[4*(i)+6] = ss[0]; k[4*(i)+7] = ss[1]; \
}
#else
#define kdf4(k,i) \
{   ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[4*(i)+ 4] = ff(ss[0]); ss[1] ^= ss[0]; k[4*(i)+ 5] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[4*(i)+ 6] = ff(ss[2]); ss[3] ^= ss[2]; k[4*(i)+ 7] = ff(ss[3]); \
}
#define kd4(k,i) \
{   ss[4] = ls_box(ss[3],3) ^ t_use(r,c)[i]; \
    ss[0] ^= ss[4]; ss[4] = ff(ss[4]); k[4*(i)+ 4] = ss[4] ^= k[4*(i)]; \
    ss[1] ^= ss[0]; k[4*(i)+ 5] = ss[4] ^= k[4*(i)+ 1]; \
    ss[2] ^= ss[1]; k[4*(i)+ 6] = ss[4] ^= k[4*(i)+ 2]; \
    ss[3] ^= ss[2]; k[4*(i)+ 7] = ss[4] ^= k[4*(i)+ 3]; \
}
#define kdl4(k,i) \
{   ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[4*(i)+ 4] = ss[0]; ss[1] ^= ss[0]; k[4*(i)+ 5] = ss[1]; \
    ss[2] ^= ss[1]; k[4*(i)+ 6] = ss[2]; ss[3] ^= ss[2]; k[4*(i)+ 7] = ss[3]; \
}
#endif

#define kdf6(k,i) \
{   ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[6*(i)+ 6] = ff(ss[0]); ss[1] ^= ss[0]; k[6*(i)+ 7] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[6*(i)+ 8] = ff(ss[2]); ss[3] ^= ss[2]; k[6*(i)+ 9] = ff(ss[3]); \
    ss[4] ^= ss[3]; k[6*(i)+10] = ff(ss[4]); ss[5] ^= ss[4]; k[6*(i)+11] = ff(ss[5]); \
}
#define kd6(k,i) \
{   ss[6] = ls_box(ss[5],3) ^ t_use(r,c)[i]; \
    ss[0] ^= ss[6]; ss[6] = ff(ss[6]); k[6*(i)+ 6] = ss[6] ^= k[6*(i)]; \
    ss[1] ^= ss[0]; k[6*(i)+ 7] = ss[6] ^= k[6*(i)+ 1]; \
    ss[2] ^= ss[1]; k[6*(i)+ 8] = ss[6] ^= k[6*(i)+ 2]; \
    ss[3] ^= ss[2]; k[6*(i)+ 9] = ss[6] ^= k[6*(i)+ 3]; \
    ss[4] ^= ss[3]; k[6*(i)+10] = ss[6] ^= k[6*(i)+ 4]; \
    ss[5] ^= ss[4]; k[6*(i)+11] = ss[6] ^= k[6*(i)+ 5]; \
}
#define kdl6(k,i) \
{   ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[6*(i)+ 6] = ss[0]; ss[1] ^= ss[0]; k[6*(i)+ 7] = ss[1]; \
    ss[2] ^= ss[1]; k[6*(i)+ 8] = ss[2]; ss[3] ^= ss[2]; k[6*(i)+ 9] = ss[3]; \
}

#define kdf8(k,i) \
{   ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[8*(i)+ 8] = ff(ss[0]); ss[1] ^= ss[0]; k[8*(i)+ 9] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[8*(i)+10] = ff(ss[2]); ss[3] ^= ss[2]; k[8*(i)+11] = ff(ss[3]); \
    ss[4] ^= ls_box(ss[3],0); k[8*(i)+12] = ff(ss[4]); ss[5] ^= ss[4]; k[8*(i)+13] = ff(ss[5]); \
    ss[6] ^= ss[5]; k[8*(i)+14] = ff(ss[6]); ss[7] ^= ss[6]; k[8*(i)+15] = ff(ss[7]); \
}
#define kd8(k,i) \
{   aes_32t g = ls_box(ss[7],3) ^ t_use(r,c)[i]; \
    ss[0] ^= g; g = ff(g); k[8*(i)+ 8] = g ^= k[8*(i)]; \
    ss[1] ^= ss[0]; k[8*(i)+ 9] = g ^= k[8*(i)+ 1]; \
    ss[2] ^= ss[1]; k[8*(i)+10] = g ^= k[8*(i)+ 2]; \
    ss[3] ^= ss[2]; k[8*(i)+11] = g ^= k[8*(i)+ 3]; \
    g = ls_box(ss[3],0); \
    ss[4] ^= g; g = ff(g); k[8*(i)+12] = g ^= k[8*(i)+ 4]; \
    ss[5] ^= ss[4]; k[8*(i)+13] = g ^= k[8*(i)+ 5]; \
    ss[6] ^= ss[5]; k[8*(i)+14] = g ^= k[8*(i)+ 6]; \
    ss[7] ^= ss[6]; k[8*(i)+15] = g ^= k[8*(i)+ 7]; \
}
#define kdl8(k,i) \
{   ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[8*(i)+ 8] = ss[0]; ss[1] ^= ss[0]; k[8*(i)+ 9] = ss[1]; \
    ss[2] ^= ss[1]; k[8*(i)+10] = ss[2]; ss[3] ^= ss[2]; k[8*(i)+11] = ss[3]; \
}

INTERNAL aes_rval aes_set_decrypt_key(const unsigned char in_key[], unsigned int klen, aes_ctx cx[1])
{   aes_32t    ss[8];
    d_vars

#if !defined(FIXED_TABLES)
#ifdef GLOBALS
    if(!t_use(in,it)) gen_tabs();
#else
    if(!cx->t_ptr || !t_use(in,it)) gen_tabs(cx);
#endif
#endif

#if !defined(BLOCK_SIZE)
    if(!cx->n_blk) cx->n_blk = 16;
#else
    cx->n_blk = BLOCK_SIZE;
#endif

    if(((klen & 7) || klen < 16 || klen > 32) && ((klen & 63) || klen < 128 || klen > 256))
    {
        cx->n_rnd = 0; return aes_bad;
    }

    klen >>= (klen < 128 ? 2 : 5);
    cx->n_blk = (cx->n_blk & ~3) | 2;

    cx->k_sch[0] = ss[0] = word_in(in_key     );
    cx->k_sch[1] = ss[1] = word_in(in_key +  4);
    cx->k_sch[2] = ss[2] = word_in(in_key +  8);
    cx->k_sch[3] = ss[3] = word_in(in_key + 12);

#if (BLOCK_SIZE == 16) && (DEC_UNROLL != NONE)

    switch(klen)
    {
    case 4:
        kdf4(cx->k_sch, 0); kd4(cx->k_sch, 1);
        kd4(cx->k_sch, 2); kd4(cx->k_sch, 3);
        kd4(cx->k_sch, 4); kd4(cx->k_sch, 5);
        kd4(cx->k_sch, 6); kd4(cx->k_sch, 7);
        kd4(cx->k_sch, 8); kdl4(cx->k_sch, 9);
        cx->n_rnd = 10; break;
    case 6:
        cx->k_sch[4] = ff(ss[4] = word_in(in_key + 16));
        cx->k_sch[5] = ff(ss[5] = word_in(in_key + 20));
        kdf6(cx->k_sch, 0); kd6(cx->k_sch, 1);
        kd6(cx->k_sch, 2); kd6(cx->k_sch, 3);
        kd6(cx->k_sch, 4); kd6(cx->k_sch, 5);
        kd6(cx->k_sch, 6); kdl6(cx->k_sch, 7);
        cx->n_rnd = 12; break;
    case 8:
        cx->k_sch[4] = ff(ss[4] = word_in(in_key + 16));
        cx->k_sch[5] = ff(ss[5] = word_in(in_key + 20));
        cx->k_sch[6] = ff(ss[6] = word_in(in_key + 24));
        cx->k_sch[7] = ff(ss[7] = word_in(in_key + 28));
        kdf8(cx->k_sch, 0); kd8(cx->k_sch, 1);
        kd8(cx->k_sch, 2); kd8(cx->k_sch, 3);
        kd8(cx->k_sch, 4); kd8(cx->k_sch, 5);
        kdl8(cx->k_sch, 6);
        cx->n_rnd = 14; break;
    default:
        ;
    }
#else
    cx->n_rnd = (klen > nc ? klen : nc) + 6;
    {   aes_32t i, l;
        l = (nc * cx->n_rnd + nc - 1) / klen;

        switch(klen)
        {
        case 4:
            for(i = 0; i < l; ++i)
                ke4(cx->k_sch, i);
            break;
        case 6:
            cx->k_sch[4] = ss[4] = word_in(in_key + 16);
            cx->k_sch[5] = ss[5] = word_in(in_key + 20);
            for(i = 0; i < l; ++i)
                ke6(cx->k_sch, i);
            break;
        case 8:
            cx->k_sch[4] = ss[4] = word_in(in_key + 16);
            cx->k_sch[5] = ss[5] = word_in(in_key + 20);
            cx->k_sch[6] = ss[6] = word_in(in_key + 24);
            cx->k_sch[7] = ss[7] = word_in(in_key + 28);
            for(i = 0; i < l; ++i)
                ke8(cx->k_sch,  i);
            break;
        default:
            ;
        }
#if (DEC_ROUND != NO_TABLES)
        for(i = nc; i < nc * cx->n_rnd; ++i)
            cx->k_sch[i] = inv_mcol(cx->k_sch[i]);
#endif
    }
#endif

    return aes_good;
}

#endif

#if defined(__cplusplus)
}
#endif
