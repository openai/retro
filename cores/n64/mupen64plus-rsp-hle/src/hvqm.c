/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-rsp-hle - hvqm.c                                          *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2020 Gilles Siberlin                                    *
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

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "hle_external.h"
#include "hle_internal.h"
#include "memory.h"

 /* Nest size  */
#define HVQM2_NESTSIZE_L 70	/* Number of elements on long side */
#define HVQM2_NESTSIZE_S 38	/* Number of elements on short side */
#define HVQM2_NESTSIZE (HVQM2_NESTSIZE_L * HVQM2_NESTSIZE_S)

struct HVQM2Block {
    uint8_t nbase;
    uint8_t dc;
    uint8_t dc_l;
    uint8_t dc_r;
    uint8_t dc_u;
    uint8_t dc_d;
};

struct HVQM2Basis {
    uint8_t sx;
    uint8_t sy;
    int16_t scale;
    uint16_t offset;
    uint16_t lineskip;
};

struct HVQM2Arg {
    uint32_t info;
    uint32_t buf;
    uint16_t buf_width;
    uint8_t chroma_step_h;
    uint8_t chroma_step_v;
    uint16_t hmcus;
    uint16_t vmcus;
    uint8_t alpha;
    uint8_t nest[HVQM2_NESTSIZE];
};

static struct HVQM2Arg arg;

static const int16_t constant[5][16] = {
{0x0006,0x0008,0x0008,0x0006,0x0008,0x000A,0x000A,0x0008,0x0008,0x000A,0x000A,0x0008,0x0006,0x0008,0x0008,0x0006},
{0x0002,0x0000,0xFFFF,0xFFFF,0x0002,0x0000,0xFFFF,0xFFFF,0x0002,0x0000,0xFFFF,0xFFFF,0x0002,0x0000,0xFFFF,0xFFFF},
{0xFFFF,0xFFFF,0x0000,0x0002,0xFFFF,0xFFFF,0x0000,0x0002,0xFFFF,0xFFFF,0x0000,0x0002,0xFFFF,0xFFFF,0x0000,0x0002},
{0x0002,0x0002,0x0002,0x0002,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF},
{0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0002,0x0002,0x0002,0x0002}
};

static int process_info(struct hle_t* hle, uint8_t* base, int16_t* out)
{
    struct HVQM2Block block;
    uint8_t nbase = *base;

    dram_load_u8(hle, (uint8_t*)&block, arg.info, sizeof(struct HVQM2Block));
    arg.info += 8;

    *base = block.nbase & 0x7;

    if ((block.nbase & nbase) != 0)
        return 0;

    if (block.nbase == 0)
    {
        //LABEL8
        for (int i = 0; i < 16; i++)
        {
            out[i] = constant[0][i] * block.dc;
            out[i] += constant[1][i] * block.dc_l;
            out[i] += constant[2][i] * block.dc_r;
            out[i] += constant[3][i] * block.dc_u;
            out[i] += constant[4][i] * block.dc_d;
            out[i] += 4;
            out[i] >>= 3;
        }
    }
    else if ((block.nbase & 0xf) == 0)
    {
        //LABEL7
        uint8_t vec[16];
        dram_load_u8(hle, vec, arg.info, 16);
        arg.info += 16;

        for (int i = 0; i < 16; i++)
            out[i] = vec[i];
    }
    else if (*base == 0)
    {
        //LABEL6
        int8_t vec[16];
        dram_load_u8(hle, (uint8_t*)vec, arg.info, 16);
        arg.info += 16;

        for (int i = 0; i < 16; i++)
            out[i] = (int16_t)vec[i] + block.dc;
    }
    else
    {
        //LABEL5
        struct HVQM2Basis basis;

        for (int i = 0; i < 16; i++)
            out[i] = block.dc;

        for (; *base != 0; (*base)--)
        {
            dram_load_u8(hle, &basis.sx, arg.info, 1);
            arg.info++;
            dram_load_u8(hle, &basis.sy, arg.info, 1);
            arg.info++;
            dram_load_u16(hle, (uint16_t*)&basis.scale, arg.info, 1);
            arg.info += 2;
            dram_load_u16(hle, &basis.offset, arg.info, 1);
            arg.info += 2;
            dram_load_u16(hle, &basis.lineskip, arg.info, 1);
            arg.info += 2;

            int16_t vec[16];
            if (basis.sx != 0)
            {
                //LABEL9
                for (int i = 0; i < 16; i += 4)
                {
                    vec[i] = arg.nest[basis.offset];
                    vec[i + 1] = arg.nest[basis.offset + 2];
                    vec[i + 2] = arg.nest[basis.offset + 4];
                    vec[i + 3] = arg.nest[basis.offset + 6];
                    basis.offset += basis.lineskip;
                }
            }
            else
            {
                //LABEL10
                for (int i = 0; i < 16; i += 4)
                {
                    vec[i] = arg.nest[basis.offset];
                    vec[i + 1] = arg.nest[basis.offset + 1];
                    vec[i + 2] = arg.nest[basis.offset + 2];
                    vec[i + 3] = arg.nest[basis.offset + 3];
                    basis.offset += basis.lineskip;
                }
            }

            //LABEL11
            int16_t sum = 0x8;
            for (int i = 0; i < 16; i++)
                sum += vec[i];

            sum >>= 4;

            int16_t max = 0;
            for (int i = 0; i < 16; i++)
            {
                vec[i] -= sum;
                max = (abs(vec[i]) > max) ? abs(vec[i]) : max;
            }

            double dmax = 0.0;
            if (max > 0)
                dmax = (double)(basis.scale << 2) / (double)max;

            for (int i = 0; i < 16; i++)
                out[i] += (vec[i] < 0) ? (int16_t)((double)vec[i] * dmax - 0.5) : (int16_t)((double)vec[i] * dmax + 0.5);

            block.nbase &= 8;
        }

        assert(block.nbase == 0);
        //if(block.nbase != 0)
        //  LABEL6
    }

    return 1;
}

#define SATURATE(x) ((unsigned int) x <= 31 ? x : (x < 0 ? 0 : 31))
static uint16_t YCbCr_to_BGRA5551(int16_t Y, int16_t Cb, int16_t Cr, uint8_t alpha)
{
    int r, g, b;

    //Format S7.9
    r = (int)(((double)Y * 0.125 + 0.0625) + (0.220703125 * (double)(Cr - 128)));
    g = (int)(((double)Y * 0.125 + 0.0625) - (0.04296875 * (double)(Cr - 128)) - (0.08984375 * (double)(Cb - 128)));
    b = (int)(((double)Y * 0.125 + 0.0625) + (0.17578125 * (double)(Cb - 128)));

    r = SATURATE(r);
    g = SATURATE(g);
    b = SATURATE(b);

    return (b << 11) | (g << 6) | (r << 1) | (alpha & 1);
}

void hvqm2_decode_sp1_task(struct hle_t* hle)
{
    //uint32_t uc_data_ptr = *dmem_u32(hle, TASK_UCODE_DATA);
    uint32_t data_ptr = *dmem_u32(hle, TASK_DATA_PTR);

    assert((*dmem_u32(hle, TASK_FLAGS) & 0x1) == 0);

    /* Fill HVQM2Arg struct */
    dram_load_u32(hle, &arg.info, data_ptr, 1);
    data_ptr += 4;
    dram_load_u32(hle, &arg.buf, data_ptr, 1);
    data_ptr += 4;
    dram_load_u16(hle, &arg.buf_width, data_ptr, 1);
    data_ptr += 2;
    dram_load_u8(hle, &arg.chroma_step_h, data_ptr, 1);
    data_ptr += 1;
    dram_load_u8(hle, &arg.chroma_step_v, data_ptr, 1);
    data_ptr += 1;
    dram_load_u16(hle, &arg.hmcus, data_ptr, 1);
    data_ptr += 2;
    dram_load_u16(hle, &arg.vmcus, data_ptr, 1);
    data_ptr += 2;
    dram_load_u8(hle, &arg.alpha, data_ptr, 1);
    data_ptr += 1;
    dram_load_u8(hle, arg.nest, data_ptr, HVQM2_NESTSIZE);

    //int length = 0x10;
    //int count = arg.chroma_step_v << 2;
    int skip = arg.buf_width << 1;

    if ((arg.chroma_step_v - 1) != 0)
    {
        assert(arg.chroma_step_v == 2);
        arg.buf_width <<= 3;
        arg.buf_width += arg.buf_width;
    }

    assert((*hle->sp_status & 0x80) == 0);  //SP_STATUS_YIELD

    for (int i = arg.vmcus; i != 0; i--)
    {
        uint32_t out;
        int j;

        for (j = arg.hmcus, out = arg.buf; j != 0; j--, out += 0x10)
        {
            uint8_t base = 0x80;
            int16_t Cb[16], Cr[16], Y1[32], Y2[32];
            int16_t* pCb = Cb;
            int16_t* pCr = Cr;
            int16_t* pY1 = Y1;
            int16_t* pY2 = Y2;

            if ((arg.chroma_step_v - 1) != 0)
            {
                if (process_info(hle, &base, pY1) == 0)
                    continue;
                if (process_info(hle, &base, pY2) == 0)
                    continue;

                pY1 = &Y1[16];
                pY2 = &Y2[16];
            }

            if (process_info(hle, &base, pY1) == 0)
                continue;
            if (process_info(hle, &base, pY2) == 0)
                continue;
            if (process_info(hle, &base, Cr) == 0)
                continue;
            if (process_info(hle, &base, Cb) == 0)
                continue;

            pY1 = Y1;
            pY2 = Y2;

            uint32_t out_buf = out;
            for (int k = 0; k < 4; k++)
            {
                for (int m = 0; m < arg.chroma_step_v; m++)
                {
                    uint32_t addr = out_buf;
                    for (int l = 0; l < 4; l++)
                    {
                        uint16_t pixel = YCbCr_to_BGRA5551(pY1[l], pCb[l >> 1], pCr[l >> 1], arg.alpha);
                        dram_store_u16(hle, &pixel, addr, 1);
                        addr += 2;
                    }
                    for (int l = 0; l < 4; l++)
                    {
                        uint16_t pixel = YCbCr_to_BGRA5551(pY2[l], pCb[(l + 4) >> 1], pCr[(l + 4) >> 1], arg.alpha);
                        dram_store_u16(hle, &pixel, addr, 1);
                        addr += 2;
                    }
                    out_buf += skip;
                    pY1 += 4;
                    pY2 += 4;
                }
                pCr += 4;
                pCb += 4;
            }
        }
        arg.buf += arg.buf_width;
    }
    rsp_break(hle, SP_STATUS_TASKDONE);
}
