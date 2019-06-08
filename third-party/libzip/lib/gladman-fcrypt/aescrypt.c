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

 This file contains the code for implementing encryption and decryption
 for AES (Rijndael) for block and key sizes of 16, 24 and 32 bytes. It
 can optionally be replaced by code written in assembler using NASM.
*/

#include "aesopt.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#if defined(BLOCK_SIZE) && (BLOCK_SIZE & 7)
#error An illegal block size has been specified.
#endif

#define libzip_fcrypt_unused  77  /* Sunset Strip */

#define si(y,x,k,c) (s(y,c) = word_in(x + 4 * c) ^ k[c])
#define so(y,x,c)   word_out(y + 4 * c, s(x,c))

#if BLOCK_SIZE == 16

#if defined(ARRAYS)
#define locals(y,x)     x[4],y[4]
#else
#define locals(y,x)     x##0,x##1,x##2,x##3,y##0,y##1,y##2,y##3
 /*
   the following defines prevent the compiler requiring the declaration
   of generated but unused variables in the fwd_var and inv_var macros
 */
#define b04 libzip_fcrypt_unused
#define b05 libzip_fcrypt_unused
#define b06 libzip_fcrypt_unused
#define b07 libzip_fcrypt_unused
#define b14 libzip_fcrypt_unused
#define b15 libzip_fcrypt_unused
#define b16 libzip_fcrypt_unused
#define b17 libzip_fcrypt_unused
#endif
#define l_copy(y, x)    s(y,0) = s(x,0); s(y,1) = s(x,1); \
                        s(y,2) = s(x,2); s(y,3) = s(x,3);
#define state_in(y,x,k) si(y,x,k,0); si(y,x,k,1); si(y,x,k,2); si(y,x,k,3)
#define state_out(y,x)  so(y,x,0); so(y,x,1); so(y,x,2); so(y,x,3)
#define round(rm,y,x,k) rm(y,x,k,0); rm(y,x,k,1); rm(y,x,k,2); rm(y,x,k,3)

#elif BLOCK_SIZE == 24

#if defined(ARRAYS)
#define locals(y,x)     x[6],y[6]
#else
#define locals(y,x)     x##0,x##1,x##2,x##3,x##4,x##5, \
                        y##0,y##1,y##2,y##3,y##4,y##5
#define b06 libzip_fcrypt_unused
#define b07 libzip_fcrypt_unused
#define b16 libzip_fcrypt_unused
#define b17 libzip_fcrypt_unused
#endif
#define l_copy(y, x)    s(y,0) = s(x,0); s(y,1) = s(x,1); \
                        s(y,2) = s(x,2); s(y,3) = s(x,3); \
                        s(y,4) = s(x,4); s(y,5) = s(x,5);
#define state_in(y,x,k) si(y,x,k,0); si(y,x,k,1); si(y,x,k,2); \
                        si(y,x,k,3); si(y,x,k,4); si(y,x,k,5)
#define state_out(y,x)  so(y,x,0); so(y,x,1); so(y,x,2); \
                        so(y,x,3); so(y,x,4); so(y,x,5)
#define round(rm,y,x,k) rm(y,x,k,0); rm(y,x,k,1); rm(y,x,k,2); \
                        rm(y,x,k,3); rm(y,x,k,4); rm(y,x,k,5)
#else

#if defined(ARRAYS)
#define locals(y,x)     x[8],y[8]
#else
#define locals(y,x)     x##0,x##1,x##2,x##3,x##4,x##5,x##6,x##7, \
                        y##0,y##1,y##2,y##3,y##4,y##5,y##6,y##7
#endif
#define l_copy(y, x)    s(y,0) = s(x,0); s(y,1) = s(x,1); \
                        s(y,2) = s(x,2); s(y,3) = s(x,3); \
                        s(y,4) = s(x,4); s(y,5) = s(x,5); \
                        s(y,6) = s(x,6); s(y,7) = s(x,7);

#if BLOCK_SIZE == 32

#define state_in(y,x,k) si(y,x,k,0); si(y,x,k,1); si(y,x,k,2); si(y,x,k,3); \
                        si(y,x,k,4); si(y,x,k,5); si(y,x,k,6); si(y,x,k,7)
#define state_out(y,x)  so(y,x,0); so(y,x,1); so(y,x,2); so(y,x,3); \
                        so(y,x,4); so(y,x,5); so(y,x,6); so(y,x,7)
#define round(rm,y,x,k) rm(y,x,k,0); rm(y,x,k,1); rm(y,x,k,2); rm(y,x,k,3); \
                        rm(y,x,k,4); rm(y,x,k,5); rm(y,x,k,6); rm(y,x,k,7)
#else

#define state_in(y,x,k) \
switch(nc) \
{   case 8: si(y,x,k,7); si(y,x,k,6); \
    case 6: si(y,x,k,5); si(y,x,k,4); \
    case 4: si(y,x,k,3); si(y,x,k,2); \
            si(y,x,k,1); si(y,x,k,0); \
}

#define state_out(y,x) \
switch(nc) \
{   case 8: so(y,x,7); so(y,x,6); \
    case 6: so(y,x,5); so(y,x,4); \
    case 4: so(y,x,3); so(y,x,2); \
            so(y,x,1); so(y,x,0); \
}

#if defined(FAST_VARIABLE)

#define round(rm,y,x,k) \
switch(nc) \
{   case 8: rm(y,x,k,7); rm(y,x,k,6); \
            rm(y,x,k,5); rm(y,x,k,4); \
            rm(y,x,k,3); rm(y,x,k,2); \
            rm(y,x,k,1); rm(y,x,k,0); \
            break; \
    case 6: rm(y,x,k,5); rm(y,x,k,4); \
            rm(y,x,k,3); rm(y,x,k,2); \
            rm(y,x,k,1); rm(y,x,k,0); \
            break; \
    case 4: rm(y,x,k,3); rm(y,x,k,2); \
            rm(y,x,k,1); rm(y,x,k,0); \
            break; \
}
#else

#define round(rm,y,x,k) \
switch(nc) \
{   case 8: rm(y,x,k,7); rm(y,x,k,6); \
    case 6: rm(y,x,k,5); rm(y,x,k,4); \
    case 4: rm(y,x,k,3); rm(y,x,k,2); \
            rm(y,x,k,1); rm(y,x,k,0); \
}

#endif

#endif
#endif

#if defined(ENCRYPTION) && !defined(AES_ASM)

/* Given the column (c) of the output state variable, the following
   macros give the input state variables which are needed in its
   computation for each row (r) of the state. All the alternative
   macros give the same end values but expand into different ways
   of calculating these values.  In particular the complex macro
   used for dynamically variable block sizes is designed to expand
   to a compile time constant whenever possible but will expand to
   conditional clauses on some branches (I am grateful to Frank
   Yellin for this construction)
*/

#if defined(BLOCK_SIZE)
#if BLOCK_SIZE == 16
# define fwd_var(x,r,c) s(x,((r+c)%nc))
#else
#define fwd_var(x,r,c) s(x,(r+c+(((r>1)&&(nc>9-r))?1:0))%nc)
#endif
#else
#define fwd_var(x,r,c)\
 ( r == 0 ?    s(x,c) \
 : r == 1 ?           \
    ( c == 0 ? s(x,1) \
    : c == 1 ? s(x,2) \
    : c == 2 ? s(x,3) \
    : c == 3 ? nc == 4 ? s(x,0) : s(x,4) \
    : c == 4 ? s(x,5) \
    : c == 5 ? nc == 8 ? s(x,6) : s(x,0) \
    : c == 6 ? s(x,7) : s(x,0)) \
 : r == 2 ? \
    ( c == 0 ? nc == 8 ? s(x,3) : s(x,2) \
    : c == 1 ? nc == 8 ? s(x,4) : s(x,3) \
    : c == 2 ? nc == 4 ? s(x,0) : nc == 8 ? s(x,5) : s(x,4) \
    : c == 3 ? nc == 4 ? s(x,1) : nc == 8 ? s(x,6) : s(x,5) \
    : c == 4 ? nc == 8 ? s(x,7) : s(x,0) \
    : c == 5 ? nc == 8 ? s(x,0) : s(x,1) \
    : c == 6 ? s(x,1) : s(x,2)) \
 : \
    ( c == 0 ? nc == 8 ? s(x,4) : s(x,3) \
    : c == 1 ? nc == 4 ? s(x,0) : nc == 8 ? s(x,5) : s(x,4) \
    : c == 2 ? nc == 4 ? s(x,1) : nc == 8 ? s(x,6) : s(x,5) \
    : c == 3 ? nc == 4 ? s(x,2) : nc == 8 ? s(x,7) : s(x,0) \
    : c == 4 ? nc == 8 ? s(x,0) : s(x,1) \
    : c == 5 ? nc == 8 ? s(x,1) : s(x,2) \
    : c == 6 ? s(x,2) : s(x,3)))
#endif

#if defined(FT4_SET)
#undef  dec_fmvars
#define dec_fmvars
#define fwd_rnd(y,x,k,c)    (s(y,c) = (k)[c] ^ four_tables(x,t_use(f,n),fwd_var,rf1,c))
#elif defined(FT1_SET)
#undef  dec_fmvars
#define dec_fmvars
#define fwd_rnd(y,x,k,c)    (s(y,c) = (k)[c] ^ one_table(x,upr,t_use(f,n),fwd_var,rf1,c))
#else
#define fwd_rnd(y,x,k,c)    (s(y,c) = fwd_mcol(no_table(x,t_use(s,box),fwd_var,rf1,c)) ^ (k)[c])
#endif

#if defined(FL4_SET)
#define fwd_lrnd(y,x,k,c)   (s(y,c) = (k)[c] ^ four_tables(x,t_use(f,l),fwd_var,rf1,c))
#elif defined(FL1_SET)
#define fwd_lrnd(y,x,k,c)   (s(y,c) = (k)[c] ^ one_table(x,ups,t_use(f,l),fwd_var,rf1,c))
#else
#define fwd_lrnd(y,x,k,c)   (s(y,c) = no_table(x,t_use(s,box),fwd_var,rf1,c) ^ (k)[c])
#endif

INTERNAL aes_rval aes_encrypt_block(const unsigned char in_blk[], unsigned char out_blk[], const aes_ctx cx[1])
{   aes_32t        locals(b0, b1);
    const aes_32t  *kp = cx->k_sch;
    dec_fmvars  /* declare variables for fwd_mcol() if needed */

    if(!(cx->n_blk & 1)) return aes_bad;

    state_in(b0, in_blk, kp);

#if (ENC_UNROLL == FULL)

    kp += (cx->n_rnd - 9) * nc;

    /*lint -e{616} control flows into case/default */
    switch(cx->n_rnd)
    {
    case 14:
        round(fwd_rnd,  b1, b0, kp - 4 * nc);
        round(fwd_rnd,  b0, b1, kp - 3 * nc);
        /* fallthrough */
    case 12:
        round(fwd_rnd,  b1, b0, kp - 2 * nc);
        round(fwd_rnd,  b0, b1, kp -     nc);
        /* fallthrough */
    case 10:
        round(fwd_rnd,  b1, b0, kp         );
        round(fwd_rnd,  b0, b1, kp +     nc);
        round(fwd_rnd,  b1, b0, kp + 2 * nc);
        round(fwd_rnd,  b0, b1, kp + 3 * nc);
        round(fwd_rnd,  b1, b0, kp + 4 * nc);
        round(fwd_rnd,  b0, b1, kp + 5 * nc);
        round(fwd_rnd,  b1, b0, kp + 6 * nc);
        round(fwd_rnd,  b0, b1, kp + 7 * nc);
        round(fwd_rnd,  b1, b0, kp + 8 * nc);
        round(fwd_lrnd, b0, b1, kp + 9 * nc);
        /* fallthrough */
    default:
        ;
    }
#else

#if (ENC_UNROLL == PARTIAL)
    {   aes_32t    rnd;
        for(rnd = 0; rnd < (cx->n_rnd >> 1) - 1; ++rnd)
        {
            kp += nc;
            round(fwd_rnd, b1, b0, kp);
            kp += nc;
            round(fwd_rnd, b0, b1, kp);
        }
        kp += nc;
        round(fwd_rnd,  b1, b0, kp);
#else
    {   aes_32t    rnd, *p0 = b0, *p1 = b1, *pt;
        for(rnd = 0; rnd < cx->n_rnd - 1; ++rnd)
        {
            kp += nc;
            round(fwd_rnd, p1, p0, kp);
            pt = p0, p0 = p1, p1 = pt;
        }
#endif
        kp += nc;
        round(fwd_lrnd, b0, b1, kp);
    }
#endif

    state_out(out_blk, b0);
    return aes_good;
}

#endif

#if defined(DECRYPTION) && !defined(AES_ASM)

/* Given the column (c) of the output state variable, the following
   macros give the input state variables which are needed in its
   computation for each row (r) of the state. All the alternative
   macros give the same end values but expand into different ways
   of calculating these values.  In particular the complex macro
   used for dynamically variable block sizes is designed to expand
   to a compile time constant whenever possible but will expand to
   conditional clauses on some branches (I am grateful to Frank
   Yellin for this construction)
*/

#if defined(BLOCK_SIZE)
#if BLOCK_SIZE == 16
#define inv_var(x,r,c) s(x,((4+c-r)%nc))
#else
#define inv_var(x,r,c) s(x,(840+c-r-(((r>1)&&(nc>9-r))?1:0))%nc)
#endif
#else
#define inv_var(x,r,c)\
 ( r == 0 ?    s(x,c) \
 : r == 1 ?           \
    ( c == 0 ? nc == 4 ? s(x,3) : nc == 8 ? s(x,7) : s(x,5) \
    : c == 1 ? s(x,0) \
    : c == 2 ? s(x,1) \
    : c == 3 ? s(x,2) \
    : c == 4 ? s(x,3) \
    : c == 5 ? s(x,4) \
    : c == 6 ? s(x,5) : s(x,6)) \
 : r == 2 ? \
    ( c == 0 ? nc == 4 ? s(x,2) : nc == 8 ? s(x,5) : s(x,4) \
    : c == 1 ? nc == 4 ? s(x,3) : nc == 8 ? s(x,6) : s(x,5) \
    : c == 2 ? nc == 8 ? s(x,7) : s(x,0) \
    : c == 3 ? nc == 8 ? s(x,0) : s(x,1) \
    : c == 4 ? nc == 8 ? s(x,1) : s(x,2) \
    : c == 5 ? nc == 8 ? s(x,2) : s(x,3) \
    : c == 6 ? s(x,3) : s(x,4)) \
 : \
    ( c == 0 ? nc == 4 ? s(x,1) : nc == 8 ? s(x,4) : s(x,3) \
    : c == 1 ? nc == 4 ? s(x,2) : nc == 8 ? s(x,5) : s(x,4) \
    : c == 2 ? nc == 4 ? s(x,3) : nc == 8 ? s(x,6) : s(x,5) \
    : c == 3 ? nc == 8 ? s(x,7) : s(x,0) \
    : c == 4 ? nc == 8 ? s(x,0) : s(x,1) \
    : c == 5 ? nc == 8 ? s(x,1) : s(x,2) \
    : c == 6 ? s(x,2) : s(x,3)))
#endif

#if defined(IT4_SET)
#undef  dec_imvars
#define dec_imvars
#define inv_rnd(y,x,k,c)    (s(y,c) = (k)[c] ^ four_tables(x,t_use(i,n),inv_var,rf1,c))
#elif defined(IT1_SET)
#undef  dec_imvars
#define dec_imvars
#define inv_rnd(y,x,k,c)    (s(y,c) = (k)[c] ^ one_table(x,upr,t_use(i,n),inv_var,rf1,c))
#else
#define inv_rnd(y,x,k,c)    (s(y,c) = inv_mcol(no_table(x,t_use(i,box),inv_var,rf1,c) ^ (k)[c]))
#endif

#if defined(IL4_SET)
#define inv_lrnd(y,x,k,c)   (s(y,c) = (k)[c] ^ four_tables(x,t_use(i,l),inv_var,rf1,c))
#elif defined(IL1_SET)
#define inv_lrnd(y,x,k,c)   (s(y,c) = (k)[c] ^ one_table(x,ups,t_use(i,l),inv_var,rf1,c))
#else
#define inv_lrnd(y,x,k,c)   (s(y,c) = no_table(x,t_use(i,box),inv_var,rf1,c) ^ (k)[c])
#endif

INTERNAL aes_rval aes_decrypt_block(const unsigned char in_blk[], unsigned char out_blk[], const aes_ctx cx[1])
{   aes_32t        locals(b0, b1);
    const aes_32t  *kp = cx->k_sch + nc * cx->n_rnd;
    dec_imvars  /* declare variables for inv_mcol() if needed */

    if(!(cx->n_blk & 2)) return aes_bad;

    state_in(b0, in_blk, kp);

#if (DEC_UNROLL == FULL)

    kp = cx->k_sch + 9 * nc;

    /*lint -e{616} control flows into case/default */
    switch(cx->n_rnd)
    {
    case 14:
        round(inv_rnd,  b1, b0, kp + 4 * nc);
        round(inv_rnd,  b0, b1, kp + 3 * nc);
    case 12:
        round(inv_rnd,  b1, b0, kp + 2 * nc);
        round(inv_rnd,  b0, b1, kp + nc    );
    case 10:
        round(inv_rnd,  b1, b0, kp         );
        round(inv_rnd,  b0, b1, kp -     nc);
        round(inv_rnd,  b1, b0, kp - 2 * nc);
        round(inv_rnd,  b0, b1, kp - 3 * nc);
        round(inv_rnd,  b1, b0, kp - 4 * nc);
        round(inv_rnd,  b0, b1, kp - 5 * nc);
        round(inv_rnd,  b1, b0, kp - 6 * nc);
        round(inv_rnd,  b0, b1, kp - 7 * nc);
        round(inv_rnd,  b1, b0, kp - 8 * nc);
        round(inv_lrnd, b0, b1, kp - 9 * nc);
    default:
        ;
    }
#else

#if (DEC_UNROLL == PARTIAL)
    {   aes_32t    rnd;
        for(rnd = 0; rnd < (cx->n_rnd >> 1) - 1; ++rnd)
        {
            kp -= nc;
            round(inv_rnd, b1, b0, kp);
            kp -= nc;
            round(inv_rnd, b0, b1, kp);
        }
        kp -= nc;
        round(inv_rnd, b1, b0, kp);
#else
    {   aes_32t    rnd, *p0 = b0, *p1 = b1, *pt;
        for(rnd = 0; rnd < cx->n_rnd - 1; ++rnd)
        {
            kp -= nc;
            round(inv_rnd, p1, p0, kp);
            pt = p0, p0 = p1, p1 = pt;
        }
#endif
        kp -= nc;
        round(inv_lrnd, b0, b1, kp);
    }
#endif

    state_out(out_blk, b0);
    return aes_good;
}

#endif

#if defined(__cplusplus)
}
#endif
