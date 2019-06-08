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
 Issue Date: 18th November 2008

 A simple file encryption application based on

    a. RFC2898 for key derivation (using HMAC-SHA1)
    b. AES in CTR mode for encryption
    c. HMAC-SHA1 for authentication
    d. A Random Data Pool based on Peter Gutmann's ideas

The command line is:

    encfile password infile

 If the last file name extension is ".enc", the file is assumed to be an
 encrypted file and an attempt is made to decrypt it with the given password,
 writing the output to a file with the same name except for the ".enc" on the
 end.  Otherwise the file is encrypted with the given password and the output
 is written to a file with the same name except that ".enc" is added on the
 end.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <windows.h>

#include "fileenc.h"
#include "prng.h"

/* error numbers    */
#define ERROR_USAGE              1
#define ERROR_PASSWORD_LENGTH    2
#define ERROR_OUT_OF_MEMORY      3
#define ERROR_INPUT_FILE         4
#define ERROR_OUTPUT_FILE        5
#define ERROR_BAD_PASSWORD       6
#define ERROR_BAD_AUTHENTICATION 7

/* error messages   */
char *err_string[] =
{
    "\nusage: encfile password infile outfile\n",
    "\npassword is too short\n",
    "\nmemory allocation has failed\n",
    "\ncannot open the input file (%s)\n",
    "\ncannot open the output file (%s)\n",
    "\nbad password\n",
    "\ndamaged file or incorrect password\n"
};

/* simple entropy collection function that uses the fast timer      */
/* since we are not using the random pool for generating secret     */
/* keys we don't need to be too worried about the entropy quality   */

/* Modified in 2008 to add revised entropy generation courtesy of   */
/* WinZip Inc. This code now performs the following sequence of     */
/* entropy generation operations on sequential calls:               */ 
/*                                                                  */
/*      - the current 8-byte Windows performance counter value      */
/*      - an 8-byte representation of the current date/time         */
/*      - an 8-byte value built from the current process ID         */
/*        and thread ID                                             */
/*      - all subsequent calls return the then-current 8-byte       */
/*        performance counter value                                 */

int entropy_fun(unsigned char buf[], unsigned int len)
{   unsigned __int64    pentium_tsc[1];
    unsigned int        i;
    static unsigned int num = 0;

    switch(num)
    {
    /* use a value that is unlikely to repeat across system reboots         */
    case 1: 
        ++num;
        GetSystemTimeAsFileTime((FILETIME *)pentium_tsc);
        break;
    /* use a value that distinguishes between different instances of this   */
    /* code that might be running on different processors at the same time  */
    case 2: 
        ++num;
        {   unsigned __int32 processtest = GetCurrentProcessId();
            unsigned __int32 threadtest  = GetCurrentThreadId();

            pentium_tsc[0] = processtest;
            pentium_tsc[0] = (pentium_tsc[0] << 32) + threadtest;
        }
        break;
    
    /* use a rapidly-changing value -- check QueryPerformanceFrequency()    */
    /* to ensure that QueryPerformanceCounter() will work                   */
    case 0: 
        ++num;
    default:
        QueryPerformanceCounter((LARGE_INTEGER *)pentium_tsc);
        break;
    }

    for(i = 0; i < 8 && i < len; ++i)
        buf[i] = ((unsigned char*)pentium_tsc)[i];
    return i;
}

/* this is the main file encryption/decryption routine code */

int main(int argc, char *argv[])
{   FILE *inf, *outf;
    unsigned char buf[1024], tmp_buf1[16], tmp_buf2[16], salt[16], *fname, *cp;
    fcrypt_ctx  zcx[1];
    int len, flen, err = 0;
    unsigned char mode;

    if(argc != 3)   /* the command line is bad  */
    {
        err = ERROR_USAGE; goto error_0;
    }


    len = (int)strlen(argv[1]);

    if(len < 8)     /* password is too short    */
    {
        err = ERROR_PASSWORD_LENGTH; goto error_0;
    }

    /* set the key length based on password length assuming that there  */
    /* are about 4 bits of entropy per password character (the key      */
    /* length and other mode dependent parameter values are set using   */
    /* macros defined in fileenc.h)                                     */
    mode = (len < 32 ? 1 : len < 48 ? 2 : 3);

    /* save input file name to a temporary memory area with extra space */
    /* for the extension ".enc" to be added                             */
    fname = (unsigned char*)malloc(strlen(argv[2]) + 5);
    if(fname == NULL)
    {
        err = ERROR_OUT_OF_MEMORY; goto error_0;
    }

    /* open the input file  */
    strcpy(fname, argv[2]);
    if((inf = fopen(fname, "rb")) == NULL)
    {
        err = ERROR_INPUT_FILE; goto error_1;
    }

    /* if the file name extension is ".enc" assume this is an encrypted */
    /* file                                                             */
    if((cp = strrchr(fname, '.')) && strcmp(cp, ".enc") == 0)
    {
        *cp = 0;
        mode |= 4;  /* signal decryption */
    }
    else                        /* add ".enc" to file name to mark the  */
        strcat(fname, ".enc");  /* the file as an encrypted one         */

    /* open output file for binary output */
    if((outf = fopen(fname, "wb")) == NULL)
    {
        err = ERROR_OUTPUT_FILE; goto error_2;
    }

    if(!(mode & 4))         /* encryption operation     */
    {
        prng_ctx rng[1];    /* the context for the random number pool   */
        prng_init(entropy_fun, rng);                /* initialise RNG   */
        prng_rand(salt, SALT_LENGTH(mode), rng);    /* and the salt     */

        /* write salt value         */
        fwrite(salt, sizeof(unsigned char), SALT_LENGTH(mode), outf);

        /* initialise encryption and authentication */
#ifdef PASSWORD_VERIFIER
        fcrypt_init(mode, argv[1], (unsigned int)strlen(argv[1]), salt, tmp_buf1, zcx);
        /* write password verifier (if used)        */
        fwrite(tmp_buf1, sizeof(unsigned char), PWD_VER_LENGTH, outf);
#else
        fcrypt_init(mode, argv[1], (unsigned int)strlen(argv[1]), salt, zcx);
#endif
        /* encrypt and authenticate the file        */
        len = (int)fread(buf, sizeof(unsigned char), 1024, inf);
        while(len)
        {
            fcrypt_encrypt(buf, len, zcx);
            fwrite(buf, sizeof(unsigned char), len, outf);
            len = (int)fread(buf, sizeof(unsigned char), len, inf);
        }

        /* write the MAC    */
        fcrypt_end(tmp_buf1, zcx);
        fwrite(tmp_buf1, sizeof(unsigned char), MAC_LENGTH(mode), outf);

        /* and close random pool    */
        prng_end(rng);
    }
    else                    /* decryption operation     */
    {
        /* we need to know the file length to avoid reading the MAC */
        fseek(inf, 0, SEEK_END);
        flen = ftell(inf);
        fseek(inf, 0, SEEK_SET);
        mode &= 3;

        /* recover the password salt     */
        fread(salt, sizeof(unsigned char), SALT_LENGTH(mode), inf); flen -= SALT_LENGTH(mode);
#ifdef  PASSWORD_VERIFIER
        /* initialise encryption and authentication */
        fcrypt_init(mode, argv[1], (unsigned int)strlen(argv[1]), salt, tmp_buf2, zcx);
        /* recover the password verifier (if used)  */
        fread(tmp_buf1, sizeof(unsigned char), PWD_VER_LENGTH, inf); flen -= PWD_VER_LENGTH;
        /* check password verifier  */
        if(memcmp(tmp_buf1, tmp_buf2, PWD_VER_LENGTH))
        {
            err = ERROR_BAD_PASSWORD; fclose(outf); goto error_2;
        }
#else
        /* initialise encryption and authentication */
        fcrypt_init(mode, argv[1], (unsigned int)strlen(argv[1]), salt, zcx);
#endif

        flen -= MAC_LENGTH(mode);   /* avoid reading the MAC    */
        /* decrypt the file     */
        len = (int)fread(buf, sizeof(unsigned char),
                    (size_t)(flen < 1024 ? flen : 1024), inf);
        while(len)
        {   flen -= len;
            fcrypt_decrypt(buf, len, zcx);
            fwrite(buf, sizeof(unsigned char), len, outf);
            len = (int)fread(buf, sizeof(unsigned char),
                        (size_t)(flen < 1024 ? flen : 1024), inf);
        }

        /* calculate the MAC value          */
        fcrypt_end(tmp_buf2, zcx);

        /* now read the stored MAC value    */
        fread(tmp_buf1, sizeof(unsigned char), MAC_LENGTH(mode), inf);

        /* compare the stored and calculated MAC values */
        if(memcmp(tmp_buf1, tmp_buf2, MAC_LENGTH(mode)))
        {   /* authentication failed        */

            err = ERROR_BAD_AUTHENTICATION;
            fclose(outf);
            /* delete the (bad) output file */
            remove(fname);
            goto error_2;
        }
    }

    fclose(outf);
error_2:
    fclose(inf);
error_1:
    free(fname);
error_0:
    if(err)
        printf(err_string[err - 1], fname);
    return -err;
}
