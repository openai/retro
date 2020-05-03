/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - m64282fp.c                                              *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2017 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Most of the mappers information comes from
 * "Game Boy Camera Technical Information v1.1.1" by AntonioND
 */

#include "m64282fp.h"

#include <string.h>

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a < b) ? b : a; }

int clamp(int v, int low, int high)
{
    return min(high, max(low, v));
}

static void do_kernel_filtering(int img[M64282FP_SENSOR_H][M64282FP_SENSOR_W], int a, const int k[6])
{
    unsigned int x, y;
    /* north line and last west pixel previous values */
    int tmp[1+M64282FP_SENSOR_W];

    memcpy(tmp, &img[0][0], M64282FP_SENSOR_W*sizeof(img[0][0]));

    for (y = 0; y < M64282FP_SENSOR_H; ++y) {

        tmp[M64282FP_SENSOR_W] = img[y][0];

        for (x = 0; x < M64282FP_SENSOR_W; ++x) {

            int px = img[y][x];
            int ms = img[min(y+1, M64282FP_SENSOR_H-1)][x];
            int me = img[y][min(x+1, M64282FP_SENSOR_W-1)];
            int mn = tmp[x];
            int mw = tmp[M64282FP_SENSOR_W];

            img[y][x] = k[0]*px+((a*(k[1]*px+k[2]*mn+k[3]*mw+k[4]*me+k[5]*ms))/4);

            tmp[x] = px;
            tmp[M64282FP_SENSOR_W] = px;
        }
    }
}

static void do_1d_filtering(int img[M64282FP_SENSOR_H][M64282FP_SENSOR_W], uint8_t P, uint8_t M)
{
    unsigned int x, y;

    for (y = 0; y < M64282FP_SENSOR_H; ++y) {
        for (x = 0; x < M64282FP_SENSOR_W; ++x) {

            int px = img[y][x];
            int s1 = img[min(y+1, M64282FP_SENSOR_H-1)][x];
            int s2 = img[min(y+2, M64282FP_SENSOR_H-1)][x];
            int s3 = img[min(y+3, M64282FP_SENSOR_H-1)][x];

            int value = 0;
            if (P & 0x01) { value += px; }
            if (P & 0x02) { value += s1; }
            if (P & 0x04) { value += s2; }
            if (P & 0x08) { value += s3; }

            if (M & 0x01) { value -= px; }
            if (M & 0x02) { value -= s1; }
            if (M & 0x04) { value -= s2; }
            if (M & 0x08) { value -= s3; }

            img[y][x] = value;
        }
    }
}


void process_m64282fp_image(
    uint8_t img[M64282FP_SENSOR_H][M64282FP_SENSOR_W],
    const uint8_t regs[M64282FP_REGS_COUNT])
{
    unsigned int x, y;
    int tmp[M64282FP_SENSOR_H][M64282FP_SENSOR_W];

    static const int kernels[6][6] = {
        /* px +a*(px +mn +mw +me +ms) */
        {   1,     2,  0, -1, -1,  0 }, /* horiz enhancement */
        {   0,     2,  0, -1, -1,  0 }, /* horiz extraction  */
        {   1,     2, -1,  0,  0, -1 }, /* vert  enhancement */
        {   0,     2, -1,  0,  0, -1 }, /* vert  extraction  */
        {   1,     4, -1, -1, -1, -1 }, /* 2d    enhancement */
        {   0,     4, -1, -1, -1, -1 }, /* 2d    extraction  */
    };

    /* edge ratio (alpha) Q.2 format */
#define Q2(x) (4*x)
    static const int alpha_lut[8] = {
        Q2(2/4), Q2(3/4), Q2(1), Q2(5/4),
        Q2(2), Q2(3), Q2(4), Q2(5)
    };
#undef Q2

    /* apply exposure effect */
    uint16_t ext_exposure = 0x0300; /* 0x0300: could be other value */
    uint16_t exposure = (regs[M64282FP_C_HI] << 8) | regs[M64282FP_C_LO];

    /* TODO: handle the zero-exposure case */

    for (y = 0; y < M64282FP_SENSOR_H; ++y) {
        for (x = 0; x < M64282FP_SENSOR_W; ++x) {
            int v = img[y][x];
            v = ((v * exposure) / ext_exposure);
            v = ((v - 128) / 8) + 128; /* adapt to 3.1V / 5V */
            img[y][x] = clamp(v, 0, 255);
        }
    }

    /* invert image when I bit is set */
    if (regs[M64282FP_E_I_V] & 0x8) {
        for (y = 0; y < M64282FP_SENSOR_H; ++y) {
            for (x = 0; x < M64282FP_SENSOR_W; ++x) {
                img[y][x] = 255 - img[y][x];
            }
        }
    }

    /* make signed */
    for (y = 0; y < M64282FP_SENSOR_H; ++y) {
        for (x = 0; x < M64282FP_SENSOR_W; ++x) {
            tmp[y][x] = img[y][x] - 128;
        }
    }

    unsigned int mode
        = (((regs[M64282FP_N_VH_G] & 0x80) >> 7) << 3)  /* N  */
        | (((regs[M64282FP_N_VH_G] & 0x60) >> 5) << 1)  /* VH */
        | (((regs[M64282FP_E_I_V]  & 0x80) >> 7) << 0); /* E3 */

    int alpha = alpha_lut[(regs[M64282FP_E_I_V] & 0x70) >> 4];

    switch(mode)
    {
    case 0x0: /* 0000: positive image */
        do_1d_filtering(tmp, regs[M64282FP_P], regs[M64282FP_M]);
        break;
    case 0x1: /* 0001: undocumented - bug ??? */
        memset(tmp, 0, sizeof(tmp));
        break;

    case 0x2: /* 0010: horiz enhancement */
        do_kernel_filtering(tmp, alpha, kernels[0]);
        do_1d_filtering(tmp, regs[M64282FP_P], regs[M64282FP_M]);
        break;
    case 0x3: /* 0011: horiz extraction */
        do_kernel_filtering(tmp, alpha, kernels[1]);
        do_1d_filtering(tmp, regs[M64282FP_P], regs[M64282FP_M]);
        break;

    case 0xc: /* 1100: vert enhancement */
        do_kernel_filtering(tmp, alpha, kernels[2]);
        break;
    case 0xd: /* 1101: vert extraction */
        do_kernel_filtering(tmp, alpha, kernels[3]);
        break;

    case 0xe: /* 1110: 2D enhancement */
        do_kernel_filtering(tmp, alpha, kernels[4]);
        break;
    case 0xf: /* 1111: 2D extraction */
        do_kernel_filtering(tmp, alpha, kernels[5]);
        break;

    default:
        break;
    }

    /* back to 0..255 range */
    for (y = 0; y < M64282FP_SENSOR_H; ++y) {
        for (x = 0; x < M64282FP_SENSOR_W; ++x) {
            img[y][x] = clamp(128 + tmp[y][x], 0, 255);
        }
    }

    /* gain and level control are not emulated */
}
