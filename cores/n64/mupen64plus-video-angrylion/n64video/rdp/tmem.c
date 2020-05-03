#define tmem16 ((uint16_t*)state[wid].tmem)
#define tc16   ((uint16_t*)state[wid].tmem)
#define tlut   ((uint16_t*)(&state[wid].tmem[0x800]))

static uint8_t replicated_rgba[32];

#define GET_LOW_RGBA16_TMEM(x)  (replicated_rgba[((x) >> 1) & 0x1f])
#define GET_MED_RGBA16_TMEM(x)  (replicated_rgba[((x) >> 6) & 0x1f])
#define GET_HI_RGBA16_TMEM(x)   (replicated_rgba[(x) >> 11])

static void sort_tmem_idx(uint32_t *idx, uint32_t idxa, uint32_t idxb, uint32_t idxc, uint32_t idxd, uint32_t bankno)
{
    if ((idxa & 3) == bankno)
        *idx = idxa & 0x3ff;
    else if ((idxb & 3) == bankno)
        *idx = idxb & 0x3ff;
    else if ((idxc & 3) == bankno)
        *idx = idxc & 0x3ff;
    else if ((idxd & 3) == bankno)
        *idx = idxd & 0x3ff;
    else
        *idx = 0;
}

static void sort_tmem_shorts_lowhalf(uint32_t* bindshort, uint32_t short0, uint32_t short1, uint32_t short2, uint32_t short3, uint32_t bankno)
{
    switch(bankno)
    {
    case 0:
        *bindshort = short0;
        break;
    case 1:
        *bindshort = short1;
        break;
    case 2:
        *bindshort = short2;
        break;
    case 3:
        *bindshort = short3;
        break;
    }
}

static void compute_color_index(uint32_t wid, uint32_t* cidx, uint32_t readshort, uint32_t nybbleoffset, uint32_t tilenum)
{
    uint32_t lownib, hinib;
    if (state[wid].tile[tilenum].size == PIXEL_SIZE_4BIT)
    {
        lownib = (nybbleoffset ^ 3) << 2;
        hinib = state[wid].tile[tilenum].palette;
    }
    else
    {
        lownib = ((nybbleoffset & 2) ^ 2) << 2;
        hinib = lownib ? ((readshort >> 12) & 0xf) : ((readshort >> 4) & 0xf);
    }
    lownib = (readshort >> lownib) & 0xf;
    *cidx = (hinib << 4) | lownib;
}

static INLINE void fetch_texel(uint32_t wid, struct color *color, int s, int t, uint32_t tilenum)
{
    uint32_t tbase = state[wid].tile[tilenum].line * (t & 0xff) + state[wid].tile[tilenum].tmem;



    uint32_t tpal   = state[wid].tile[tilenum].palette;








    uint32_t taddr = 0;





    switch (state[wid].tile[tilenum].f.notlutswitch)
    {
    case TEXEL_RGBA4:
        {
            taddr = ((tbase << 4) + s) >> 1;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);
            uint8_t byteval, c;

            byteval = state[wid].tmem[taddr & 0xfff];
            c = ((s & 1)) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color->r = c;
            color->g = c;
            color->b = c;
            color->a = c;
        }
        break;
    case TEXEL_RGBA8:
        {
            taddr = (tbase << 3) + s;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t p;

            p = state[wid].tmem[taddr & 0xfff];
            color->r = p;
            color->g = p;
            color->b = p;
            color->a = p;
        }
        break;
    case TEXEL_RGBA16:
        {
            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);


            uint16_t c;

            c = tc16[taddr & 0x7ff];
            color->r = GET_HI_RGBA16_TMEM(c);
            color->g = GET_MED_RGBA16_TMEM(c);
            color->b = GET_LOW_RGBA16_TMEM(c);
            color->a = (c & 1) ? 0xff : 0;
        }
        break;
    case TEXEL_RGBA32:
        {



            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            uint16_t c;


            taddr &= 0x3ff;
            c = tc16[taddr];
            color->r = c >> 8;
            color->g = c & 0xff;
            c = tc16[taddr | 0x400];
            color->b = c >> 8;
            color->a = c & 0xff;
        }
        break;
    case TEXEL_YUV4:
        {
            taddr = (tbase << 3) + s;

            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            int32_t u, save;

            save = state[wid].tmem[taddr & 0x7ff];

            save &= 0xf0;
            save |= (save >> 4);

            u = save - 0x80;

            color->r = u;
            color->g = u;
            color->b = save;
            color->a = save;
        }
        break;
    case TEXEL_YUV8:
        {
            taddr = (tbase << 3) + s;

            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            int32_t u, save;

            save = u = state[wid].tmem[taddr & 0x7ff];

            u = u - 0x80;

            color->r = u;
            color->g = u;
            color->b = save;
            color->a = save;
        }
        break;
    case TEXEL_YUV16:
        {
            taddr = (tbase << 3) + s;
            int taddrlow = taddr >> 1;

            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);
            taddrlow ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            taddr &= 0x7ff;
            taddrlow &= 0x3ff;

            uint16_t c = tc16[taddrlow];

            int32_t y, u, v;
            y = state[wid].tmem[taddr | 0x800];
            u = c >> 8;
            v = c & 0xff;

            u = u - 0x80;
            v = v - 0x80;



            color->r = u;
            color->g = v;
            color->b = y;
            color->a = y;
        }
        break;
    case TEXEL_YUV32:
        {
            int taddrlow;
            uint16_t c;
            int32_t y, u, v;

            taddr = (tbase << 3) + s;
            taddrlow = taddr >> 1;

            taddrlow ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            taddrlow &= 0x3ff;

            c = tc16[taddrlow];

            u = c >> 8;
            v = c & 0xff;

            u = u - 0x80;
            v = v - 0x80;

            color->r = u;
            color->g = v;

            if (s & 1)
            {
                taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);
                taddr &= 0x7ff;
                y = state[wid].tmem[taddr | 0x800];

                color->b = y;
                color->a = y;
            }
            else
            {
                y = tc16[taddrlow | 0x400];

                color->b = y >> 8;
                color->a = ((y >> 8) & 0xf) | (y & 0xf0);
            }
        }
        break;
    case TEXEL_CI4:
        {
            taddr = ((tbase << 4) + s) >> 1;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t p;



            p = state[wid].tmem[taddr & 0xfff];
            p = (s & 1) ? (p & 0xf) : (p >> 4);
            p = (tpal << 4) | p;
            color->r = color->g = color->b = color->a = p;
        }
        break;
    case TEXEL_CI8:
        {
            taddr = (tbase << 3) + s;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t p;


            p = state[wid].tmem[taddr & 0xfff];
            color->r = p;
            color->g = p;
            color->b = p;
            color->a = p;
        }
        break;
    case TEXEL_CI16:
    case TEXEL_CI32:
        {
            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            uint16_t c;

            c = tc16[taddr & 0x7ff];
            color->r = c >> 8;
            color->g = c & 0xff;
            color->b = color->r;
            color->a = color->g;
        }
        break;
    case TEXEL_IA4:
        {
            taddr = ((tbase << 4) + s) >> 1;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t p, i;


            p = state[wid].tmem[taddr & 0xfff];
            p = (s & 1) ? (p & 0xf) : (p >> 4);
            i = p & 0xe;
            i = (i << 4) | (i << 1) | (i >> 2);
            color->r = i;
            color->g = i;
            color->b = i;
            color->a = (p & 0x1) ? 0xff : 0;
        }
        break;
    case TEXEL_IA8:
        {
            taddr = (tbase << 3) + s;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t p, i;


            p = state[wid].tmem[taddr & 0xfff];
            i = p & 0xf0;
            i |= (i >> 4);
            color->r = i;
            color->g = i;
            color->b = i;
            color->a = ((p & 0xf) << 4) | (p & 0xf);
        }
        break;
    case TEXEL_IA16:
        {


            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            uint16_t c;

            c = tc16[taddr & 0x7ff];
            color->r = color->g = color->b = (c >> 8);
            color->a = c & 0xff;
        }
        break;
    case TEXEL_IA32:
        {
            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            uint16_t c;

            c = tc16[taddr & 0x7ff];
            color->r = c >> 8;
            color->g = c & 0xff;
            color->b = color->r;
            color->a = color->g;
        }
        break;
    case TEXEL_I4:
        {
            taddr = ((tbase << 4) + s) >> 1;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t byteval, c;

            byteval = state[wid].tmem[taddr & 0xfff];
            c = (s & 1) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color->r = c;
            color->g = c;
            color->b = c;
            color->a = c;
        }
        break;
    case TEXEL_I8:
        {
            taddr = (tbase << 3) + s;
            taddr ^= ((t & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR);

            uint8_t c;

            c = state[wid].tmem[taddr & 0xfff];
            color->r = c;
            color->g = c;
            color->b = c;
            color->a = c;
        }
        break;
    case TEXEL_I16:
    case TEXEL_I32:
    default:
        {
            taddr = (tbase << 2) + s;
            taddr ^= ((t & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR);

            uint16_t c;

            c = tc16[taddr & 0x7ff];
            color->r = c >> 8;
            color->g = c & 0xff;
            color->b = color->r;
            color->a = color->g;
        }
        break;
    }
}

static INLINE void fetch_texel_quadro(uint32_t wid, struct color *color0, struct color *color1, struct color *color2, struct color *color3, int s0, int sdiff, int t0, int tdiff, uint32_t tilenum, int unequaluppers)
{

    uint32_t tbase0 = state[wid].tile[tilenum].line * (t0 & 0xff) + state[wid].tile[tilenum].tmem;

    int t1 = (t0 & 0xff) + tdiff;



    int s1 = s0 + sdiff;

    uint32_t tbase2 = state[wid].tile[tilenum].line * t1 + state[wid].tile[tilenum].tmem;
    uint32_t tpal = state[wid].tile[tilenum].palette;
    uint32_t xort, ands;




    uint32_t taddr0, taddr1, taddr2, taddr3;
    uint32_t taddrlow0, taddrlow1, taddrlow2, taddrlow3;

    switch (state[wid].tile[tilenum].f.notlutswitch)
    {
    case TEXEL_RGBA4:
        {
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            taddr1 = ((tbase0 << 4) + s1) >> 1;
            taddr2 = ((tbase2 << 4) + s0) >> 1;
            taddr3 = ((tbase2 << 4) + s1) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t byteval, c;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            ands = s0 & 1;
            byteval = state[wid].tmem[taddr0];
            c = (ands) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color0->r = c;
            color0->g = c;
            color0->b = c;
            color0->a = c;
            byteval = state[wid].tmem[taddr2];
            c = (ands) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color2->r = c;
            color2->g = c;
            color2->b = c;
            color2->a = c;

            ands = s1 & 1;
            byteval = state[wid].tmem[taddr1];
            c = (ands) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color1->r = c;
            color1->g = c;
            color1->b = c;
            color1->a = c;
            byteval = state[wid].tmem[taddr3];
            c = (ands) ? (byteval & 0xf) : (byteval >> 4);
            c |= (c << 4);
            color3->r = c;
            color3->g = c;
            color3->b = c;
            color3->a = c;
        }
        break;
    case TEXEL_RGBA8:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            p = state[wid].tmem[taddr0];
            color0->r = p;
            color0->g = p;
            color0->b = p;
            color0->a = p;
            p = state[wid].tmem[taddr2];
            color2->r = p;
            color2->g = p;
            color2->b = p;
            color2->a = p;
            p = state[wid].tmem[taddr1];
            color1->r = p;
            color1->g = p;
            color1->b = p;
            color1->a = p;
            p = state[wid].tmem[taddr3];
            color3->r = p;
            color3->g = p;
            color3->b = p;
            color3->a = p;
        }
        break;
    case TEXEL_RGBA16:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t c0, c1, c2, c3;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            c0 = tc16[taddr0];
            c1 = tc16[taddr1];
            c2 = tc16[taddr2];
            c3 = tc16[taddr3];
            color0->r = GET_HI_RGBA16_TMEM(c0);
            color0->g = GET_MED_RGBA16_TMEM(c0);
            color0->b = GET_LOW_RGBA16_TMEM(c0);
            color0->a = (c0 & 1) ? 0xff : 0;
            color1->r = GET_HI_RGBA16_TMEM(c1);
            color1->g = GET_MED_RGBA16_TMEM(c1);
            color1->b = GET_LOW_RGBA16_TMEM(c1);
            color1->a = (c1 & 1) ? 0xff : 0;
            color2->r = GET_HI_RGBA16_TMEM(c2);
            color2->g = GET_MED_RGBA16_TMEM(c2);
            color2->b = GET_LOW_RGBA16_TMEM(c2);
            color2->a = (c2 & 1) ? 0xff : 0;
            color3->r = GET_HI_RGBA16_TMEM(c3);
            color3->g = GET_MED_RGBA16_TMEM(c3);
            color3->b = GET_LOW_RGBA16_TMEM(c3);
            color3->a = (c3 & 1) ? 0xff : 0;
        }
        break;
    case TEXEL_RGBA32:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint16_t c0, c1, c2, c3;

            taddr0 &= 0x3ff;
            taddr1 &= 0x3ff;
            taddr2 &= 0x3ff;
            taddr3 &= 0x3ff;
            c0 = tc16[taddr0];
            color0->r = c0 >> 8;
            color0->g = c0 & 0xff;
            c0 = tc16[taddr0 | 0x400];
            color0->b = c0 >>  8;
            color0->a = c0 & 0xff;
            c1 = tc16[taddr1];
            color1->r = c1 >> 8;
            color1->g = c1 & 0xff;
            c1 = tc16[taddr1 | 0x400];
            color1->b = c1 >>  8;
            color1->a = c1 & 0xff;
            c2 = tc16[taddr2];
            color2->r = c2 >> 8;
            color2->g = c2 & 0xff;
            c2 = tc16[taddr2 | 0x400];
            color2->b = c2 >>  8;
            color2->a = c2 & 0xff;
            c3 = tc16[taddr3];
            color3->r = c3 >> 8;
            color3->g = c3 & 0xff;
            c3 = tc16[taddr3 | 0x400];
            color3->b = c3 >>  8;
            color3->a = c3 & 0xff;
        }
        break;
    case TEXEL_YUV4:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1 + sdiff;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1 + sdiff;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;

            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            int32_t u0, u1, u2, u3, save0, save1, save2, save3;

            save0 = state[wid].tmem[taddr0 & 0x7ff];
            save0 &= 0xf0;
            save0 |= (save0 >> 4);
            u0 = save0 - 0x80;

            save1 = state[wid].tmem[taddr1 & 0x7ff];
            save1 &= 0xf0;
            save1 |= (save1 >> 4);
            u1 = save1 - 0x80;

            save2 = state[wid].tmem[taddr2 & 0x7ff];
            save2 &= 0xf0;
            save2 |= (save2 >> 4);
            u2 = save2 - 0x80;

            save3 = state[wid].tmem[taddr3 & 0x7ff];
            save3 &= 0xf0;
            save3 |= (save3 >> 4);
            u3 = save3 - 0x80;

            color0->r = u0;
            color0->g = u0;
            color1->r = u1;
            color1->g = u1;
            color2->r = u2;
            color2->g = u2;
            color3->r = u3;
            color3->g = u3;

            if (unequaluppers)
            {
                color0->b = color0->a = save3;
                color1->b = color1->a = save2;
                color2->b = color2->a = save1;
                color3->b = color3->a = save0;
            }
            else
            {
                color0->b = color0->a = save0;
                color1->b = color1->a = save1;
                color2->b = color2->a = save2;
                color3->b = color3->a = save3;
            }
        }
        break;
    case TEXEL_YUV8:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1 + sdiff;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1 + sdiff;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            int32_t u0, u1, u2, u3, save0, save1, save2, save3;

            save0 = u0 = state[wid].tmem[taddr0 & 0x7ff];
            u0 = u0 - 0x80;
            save1 = u1 = state[wid].tmem[taddr1 & 0x7ff];
            u1 = u1 - 0x80;
            save2 = u2 = state[wid].tmem[taddr2 & 0x7ff];
            u2 = u2 - 0x80;
            save3 = u3 = state[wid].tmem[taddr3 & 0x7ff];
            u3 = u3 - 0x80;

            color0->r = u0;
            color0->g = u0;
            color1->r = u1;
            color1->g = u1;
            color2->r = u2;
            color2->g = u2;
            color3->r = u3;
            color3->g = u3;

            if (unequaluppers)
            {
                color0->b = color0->a = save3;
                color1->b = color1->a = save2;
                color2->b = color2->a = save1;
                color3->b = color3->a = save0;
            }
            else
            {
                color0->b = color0->a = save0;
                color1->b = color1->a = save1;
                color2->b = color2->a = save2;
                color3->b = color3->a = save3;
            }
        }
        break;
    case TEXEL_YUV16:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;


            taddrlow0 = (taddr0) >> 1;
            taddrlow1 = (taddr1 + sdiff) >> 1;
            taddrlow2 = (taddr2) >> 1;
            taddrlow3 = (taddr3 + sdiff) >> 1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddrlow0 ^= xort;
            taddrlow1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddrlow2 ^= xort;
            taddrlow3 ^= xort;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            taddrlow0 &= 0x3ff;
            taddrlow1 &= 0x3ff;
            taddrlow2 &= 0x3ff;
            taddrlow3 &= 0x3ff;

            uint16_t c0, c1, c2, c3;
            int32_t y0, y1, y2, y3, u0, u1, u2, u3, v0, v1, v2, v3;

            c0 = tc16[taddrlow0];
            c1 = tc16[taddrlow1];
            c2 = tc16[taddrlow2];
            c3 = tc16[taddrlow3];

            y0 = state[wid].tmem[taddr0 | 0x800];
            u0 = c0 >> 8;
            v0 = c0 & 0xff;
            y1 = state[wid].tmem[taddr1 | 0x800];
            u1 = c1 >> 8;
            v1 = c1 & 0xff;
            y2 = state[wid].tmem[taddr2 | 0x800];
            u2 = c2 >> 8;
            v2 = c2 & 0xff;
            y3 = state[wid].tmem[taddr3 | 0x800];
            u3 = c3 >> 8;
            v3 = c3 & 0xff;

            u0 = u0 - 0x80;
            v0 = v0 - 0x80;
            u1 = u1 - 0x80;
            v1 = v1 - 0x80;
            u2 = u2 - 0x80;
            v2 = v2 - 0x80;
            u3 = u3 - 0x80;
            v3 = v3 - 0x80;

            color0->r = u0;
            color0->g = v0;
            color1->r = u1;
            color1->g = v1;
            color2->r = u2;
            color2->g = v2;
            color3->r = u3;
            color3->g = v3;

            color0->b = color0->a = y0;
            color1->b = color1->a = y1;
            color2->b = color2->a = y2;
            color3->b = color3->a = y3;
        }
        break;
    case TEXEL_YUV32:
        {
            uint16_t c0, c1, c2, c3;
            int32_t y0, y1, y2, y3, u0, u1, u2, u3, v0, v1, v2, v3;
            uint32_t xort0, xort1;

            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            taddrlow0 = (taddr0) >> 1;
            taddrlow1 = (taddr1 + sdiff) >> 1;
            taddrlow2 = (taddr2) >> 1;
            taddrlow3 = (taddr3 + sdiff) >> 1;

            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddrlow0 ^= xort;
            taddrlow1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddrlow2 ^= xort;
            taddrlow3 ^= xort;

            taddrlow0 &= 0x3ff;
            taddrlow1 &= 0x3ff;
            taddrlow2 &= 0x3ff;
            taddrlow3 &= 0x3ff;

            c0 = tc16[taddrlow0];
            c1 = tc16[taddrlow1];
            c2 = tc16[taddrlow2];
            c3 = tc16[taddrlow3];

            u0 = c0 >> 8;
            v0 = c0 & 0xff;
            u1 = c1 >> 8;
            v1 = c1 & 0xff;
            u2 = c2 >> 8;
            v2 = c2 & 0xff;
            u3 = c3 >> 8;
            v3 = c3 & 0xff;

            u0 = u0 - 0x80;
            v0 = v0 - 0x80;
            u1 = u1 - 0x80;
            v1 = v1 - 0x80;
            u2 = u2 - 0x80;
            v2 = v2 - 0x80;
            u3 = u3 - 0x80;
            v3 = v3 - 0x80;

            color0->r = u0;
            color0->g = v0;
            color1->r = u1;
            color1->g = v1;
            color2->r = u2;
            color2->g = v2;
            color3->r = u3;
            color3->g = v3;

            xort0 = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            xort1 = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;

            if (s0 & 1)
            {
                taddr0 ^= xort0;
                taddr2 ^= xort1;

                taddr0 &= 0x7ff;
                taddr2 &= 0x7ff;

                y0 = state[wid].tmem[taddr0 | 0x800];
                y2 = state[wid].tmem[taddr2 | 0x800];

                color0->b = color0->a = y0;
                color2->b = color2->a = y2;
            }
            else
            {
                y0 = tc16[taddrlow0 | 0x400];
                y2 = tc16[taddrlow2 | 0x400];

                color0->b = y0 >> 8;
                color0->a = ((y0 >> 8) & 0xf) | (y0 & 0xf0);
                color2->b = y2 >> 8;
                color2->a = ((y2 >> 8) & 0xf) | (y2 & 0xf0);
            }

            if (s1 & 1)
            {
                taddr1 ^= xort0;
                taddr3 ^= xort1;

                taddr1 &= 0x7ff;
                taddr3 &= 0x7ff;

                y1 = state[wid].tmem[taddr1 | 0x800];
                y3 = state[wid].tmem[taddr3 | 0x800];

                color1->b = color1->a = y1;
                color3->b = color3->a = y3;
            }
            else
            {
                taddr1 ^= xort0;
                taddr3 ^= xort1;

                taddr1 = (taddr1 >> 1) & 0x3ff;
                taddr3 = (taddr3 >> 1) & 0x3ff;

                y1 = tc16[taddr1 | 0x400];
                y3 = tc16[taddr3 | 0x400];

                color1->b = y1 >> 8;
                color1->a = ((y1 >> 8) & 0xf) | (y1 & 0xf0);
                color3->b = y3 >> 8;
                color3->a = ((y3 >> 8) & 0xf) | (y3 & 0xf0);
            }
        }
        break;
    case TEXEL_CI4:
        {
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            taddr1 = ((tbase0 << 4) + s1) >> 1;
            taddr2 = ((tbase2 << 4) + s0) >> 1;
            taddr3 = ((tbase2 << 4) + s1) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            ands = s0 & 1;
            p = state[wid].tmem[taddr0];
            p = (ands) ? (p & 0xf) : (p >> 4);
            p = (tpal << 4) | p;
            color0->r = color0->g = color0->b = color0->a = p;
            p = state[wid].tmem[taddr2];
            p = (ands) ? (p & 0xf) : (p >> 4);
            p = (tpal << 4) | p;
            color2->r = color2->g = color2->b = color2->a = p;

            ands = s1 & 1;
            p = state[wid].tmem[taddr1];
            p = (ands) ? (p & 0xf) : (p >> 4);
            p = (tpal << 4) | p;
            color1->r = color1->g = color1->b = color1->a = p;
            p = state[wid].tmem[taddr3];
            p = (ands) ? (p & 0xf) : (p >> 4);
            p = (tpal << 4) | p;
            color3->r = color3->g = color3->b = color3->a = p;
        }
        break;
    case TEXEL_CI8:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            p = state[wid].tmem[taddr0];
            color0->r = p;
            color0->g = p;
            color0->b = p;
            color0->a = p;
            p = state[wid].tmem[taddr2];
            color2->r = p;
            color2->g = p;
            color2->b = p;
            color2->a = p;
            p = state[wid].tmem[taddr1];
            color1->r = p;
            color1->g = p;
            color1->b = p;
            color1->a = p;
            p = state[wid].tmem[taddr3];
            color3->r = p;
            color3->g = p;
            color3->b = p;
            color3->a = p;
        }
        break;
    case TEXEL_CI16:
    case TEXEL_CI32:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint16_t c0, c1, c2, c3;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            c0 = tc16[taddr0];
            color0->r = c0 >> 8;
            color0->g = c0 & 0xff;
            color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            c1 = tc16[taddr1];
            color1->r = c1 >> 8;
            color1->g = c1 & 0xff;
            color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            c2 = tc16[taddr2];
            color2->r = c2 >> 8;
            color2->g = c2 & 0xff;
            color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            c3 = tc16[taddr3];
            color3->r = c3 >> 8;
            color3->g = c3 & 0xff;
            color3->b = c3 >> 8;
            color3->a = c3 & 0xff;
        }
        break;
    case TEXEL_IA4:
        {
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            taddr1 = ((tbase0 << 4) + s1) >> 1;
            taddr2 = ((tbase2 << 4) + s0) >> 1;
            taddr3 = ((tbase2 << 4) + s1) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p, i;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            ands = s0 & 1;
            p = state[wid].tmem[taddr0];
            p = ands ? (p & 0xf) : (p >> 4);
            i = p & 0xe;
            i = (i << 4) | (i << 1) | (i >> 2);
            color0->r = i;
            color0->g = i;
            color0->b = i;
            color0->a = (p & 0x1) ? 0xff : 0;
            p = state[wid].tmem[taddr2];
            p = ands ? (p & 0xf) : (p >> 4);
            i = p & 0xe;
            i = (i << 4) | (i << 1) | (i >> 2);
            color2->r = i;
            color2->g = i;
            color2->b = i;
            color2->a = (p & 0x1) ? 0xff : 0;

            ands = s1 & 1;
            p = state[wid].tmem[taddr1];
            p = ands ? (p & 0xf) : (p >> 4);
            i = p & 0xe;
            i = (i << 4) | (i << 1) | (i >> 2);
            color1->r = i;
            color1->g = i;
            color1->b = i;
            color1->a = (p & 0x1) ? 0xff : 0;
            p = state[wid].tmem[taddr3];
            p = ands ? (p & 0xf) : (p >> 4);
            i = p & 0xe;
            i = (i << 4) | (i << 1) | (i >> 2);
            color3->r = i;
            color3->g = i;
            color3->b = i;
            color3->a = (p & 0x1) ? 0xff : 0;
        }
        break;
    case TEXEL_IA8:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p, i;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            p = state[wid].tmem[taddr0];
            i = p & 0xf0;
            i |= (i >> 4);
            color0->r = i;
            color0->g = i;
            color0->b = i;
            color0->a = ((p & 0xf) << 4) | (p & 0xf);
            p = state[wid].tmem[taddr1];
            i = p & 0xf0;
            i |= (i >> 4);
            color1->r = i;
            color1->g = i;
            color1->b = i;
            color1->a = ((p & 0xf) << 4) | (p & 0xf);
            p = state[wid].tmem[taddr2];
            i = p & 0xf0;
            i |= (i >> 4);
            color2->r = i;
            color2->g = i;
            color2->b = i;
            color2->a = ((p & 0xf) << 4) | (p & 0xf);
            p = state[wid].tmem[taddr3];
            i = p & 0xf0;
            i |= (i >> 4);
            color3->r = i;
            color3->g = i;
            color3->b = i;
            color3->a = ((p & 0xf) << 4) | (p & 0xf);
        }
        break;
    case TEXEL_IA16:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint16_t c0, c1, c2, c3;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            c0 = tc16[taddr0];
            color0->r = color0->g = color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            c1 = tc16[taddr1];
            color1->r = color1->g = color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            c2 = tc16[taddr2];
            color2->r = color2->g = color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            c3 = tc16[taddr3];
            color3->r = color3->g = color3->b = c3 >> 8;
            color3->a = c3 & 0xff;

        }
        break;
    case TEXEL_IA32:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint16_t c0, c1, c2, c3;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            c0 = tc16[taddr0];
            color0->r = c0 >> 8;
            color0->g = c0 & 0xff;
            color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            c1 = tc16[taddr1];
            color1->r = c1 >> 8;
            color1->g = c1 & 0xff;
            color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            c2 = tc16[taddr2];
            color2->r = c2 >> 8;
            color2->g = c2 & 0xff;
            color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            c3 = tc16[taddr3];
            color3->r = c3 >> 8;
            color3->g = c3 & 0xff;
            color3->b = c3 >> 8;
            color3->a = c3 & 0xff;

        }
        break;
    case TEXEL_I4:
        {
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            taddr1 = ((tbase0 << 4) + s1) >> 1;
            taddr2 = ((tbase2 << 4) + s0) >> 1;
            taddr3 = ((tbase2 << 4) + s1) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p, c0, c1, c2, c3;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;
            ands = s0 & 1;
            p = state[wid].tmem[taddr0];
            c0 = ands ? (p & 0xf) : (p >> 4);
            c0 |= (c0 << 4);
            color0->r = color0->g = color0->b = color0->a = c0;
            p = state[wid].tmem[taddr2];
            c2 = ands ? (p & 0xf) : (p >> 4);
            c2 |= (c2 << 4);
            color2->r = color2->g = color2->b = color2->a = c2;

            ands = s1 & 1;
            p = state[wid].tmem[taddr1];
            c1 = ands ? (p & 0xf) : (p >> 4);
            c1 |= (c1 << 4);
            color1->r = color1->g = color1->b = color1->a = c1;
            p = state[wid].tmem[taddr3];
            c3 = ands ? (p & 0xf) : (p >> 4);
            c3 |= (c3 << 4);
            color3->r = color3->g = color3->b = color3->a = c3;

        }
        break;
    case TEXEL_I8:
        {
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint32_t p;

            taddr0 &= 0xfff;
            taddr1 &= 0xfff;
            taddr2 &= 0xfff;
            taddr3 &= 0xfff;

            p = state[wid].tmem[taddr0];
            color0->r = p;
            color0->g = p;
            color0->b = p;
            color0->a = p;
            p = state[wid].tmem[taddr1];
            color1->r = p;
            color1->g = p;
            color1->b = p;
            color1->a = p;
            p = state[wid].tmem[taddr2];
            color2->r = p;
            color2->g = p;
            color2->b = p;
            color2->a = p;
            p = state[wid].tmem[taddr3];
            color3->r = p;
            color3->g = p;
            color3->b = p;
            color3->a = p;
        }
        break;
    case TEXEL_I16:
    case TEXEL_I32:
    default:
        {
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            uint16_t c0, c1, c2, c3;

            taddr0 &= 0x7ff;
            taddr1 &= 0x7ff;
            taddr2 &= 0x7ff;
            taddr3 &= 0x7ff;
            c0 = tc16[taddr0];
            color0->r = c0 >> 8;
            color0->g = c0 & 0xff;
            color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            c1 = tc16[taddr1];
            color1->r = c1 >> 8;
            color1->g = c1 & 0xff;
            color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            c2 = tc16[taddr2];
            color2->r = c2 >> 8;
            color2->g = c2 & 0xff;
            color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            c3 = tc16[taddr3];
            color3->r = c3 >> 8;
            color3->g = c3 & 0xff;
            color3->b = c3 >> 8;
            color3->a = c3 & 0xff;
        }
        break;
    }
}

static INLINE void fetch_texel_entlut_quadro(uint32_t wid, struct color *color0, struct color *color1, struct color *color2, struct color *color3, int s0, int sdiff, int t0, int tdiff, uint32_t tilenum, int isupper, int isupperrg)
{
    uint32_t tbase0 = state[wid].tile[tilenum].line * (t0 & 0xff) + state[wid].tile[tilenum].tmem;
    int t1 = (t0 & 0xff) + tdiff;
    int s1;

    uint32_t tbase2 = state[wid].tile[tilenum].line * t1 + state[wid].tile[tilenum].tmem;
    uint32_t tpal = state[wid].tile[tilenum].palette << 4;
    uint32_t xort, ands;

    uint32_t taddr0, taddr1, taddr2, taddr3;
    uint16_t c0, c1, c2, c3;




    uint32_t xorupperrg = isupperrg ? (WORD_ADDR_XOR ^ 3) : WORD_ADDR_XOR;



    switch(state[wid].tile[tilenum].f.tlutswitch)
    {
    case 0:
    case 1:
    case 2:
        {
            s1 = s0 + sdiff;
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            taddr1 = ((tbase0 << 4) + s1) >> 1;
            taddr2 = ((tbase2 << 4) + s0) >> 1;
            taddr3 = ((tbase2 << 4) + s1) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            ands = s0 & 1;
            c0 = state[wid].tmem[taddr0 & 0x7ff];
            c0 = (ands) ? (c0 & 0xf) : (c0 >> 4);
            taddr0 = (tpal | c0) << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            c2 = (ands) ? (c2 & 0xf) : (c2 >> 4);
            taddr2 = ((tpal | c2) << 2) + 2;

            ands = s1 & 1;
            c1 = state[wid].tmem[taddr1 & 0x7ff];
            c1 = (ands) ? (c1 & 0xf) : (c1 >> 4);
            taddr1 = ((tpal | c1) << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            c3 = (ands) ? (c3 & 0xf) : (c3 >> 4);
            taddr3 = ((tpal | c3) << 2) + 3;
        }
        break;
    case 3:
        {
            s1 = s0 + (sdiff << 1);
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];
            c0 >>= 4;
            taddr0 = (tpal | c0) << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            c2 >>= 4;
            taddr2 = ((tpal | c2) << 2) + 2;

            c1 = state[wid].tmem[taddr1 & 0x7ff];
            c1 >>= 4;
            taddr1 = ((tpal | c1) << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            c3 >>= 4;
            taddr3 = ((tpal | c3) << 2) + 3;
        }
        break;
    case 4:
    case 5:
    case 6:
        {
            s1 = s0 + sdiff;
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];
            taddr0 = c0 << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            taddr2 = (c2 << 2) + 2;
            c1 = state[wid].tmem[taddr1 & 0x7ff];
            taddr1 = (c1 << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            taddr3 = (c3 << 2) + 3;
        }
        break;
    case 7:
        {
            s1 = s0 + (sdiff << 1);
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];
            taddr0 = c0 << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            taddr2 = (c2 << 2) + 2;
            c1 = state[wid].tmem[taddr1 & 0x7ff];
            taddr1 = (c1 << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            taddr3 = (c3 << 2) + 3;
        }
        break;
    case 8:
    case 9:
    case 10:
        {
            s1 = s0 + sdiff;
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = tc16[taddr0 & 0x3ff];
            taddr0 = (c0 >> 6) & ~3;
            c1 = tc16[taddr1 & 0x3ff];
            taddr1 = ((c1 >> 6) & ~3) + 1;
            c2 = tc16[taddr2 & 0x3ff];
            taddr2 = ((c2 >> 6) & ~3) + 2;
            c3 = tc16[taddr3 & 0x3ff];
            taddr3 = (c3 >> 6) | 3;
        }
        break;
    case 11:
        {
            s1 = s0 + (sdiff << 1);
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];
            taddr0 = c0 << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            taddr2 = (c2 << 2) + 2;
            c1 = state[wid].tmem[taddr1 & 0x7ff];
            taddr1 = (c1 << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            taddr3 = (c3 << 2) + 3;
        }
        break;
    case 12:
    case 13:
    case 14:
        {
            s1 = s0 + sdiff;
            taddr0 = (tbase0 << 2) + s0;
            taddr1 = (tbase0 << 2) + s1;
            taddr2 = (tbase2 << 2) + s0;
            taddr3 = (tbase2 << 2) + s1;

            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = tc16[taddr0 & 0x3ff];
            taddr0 = (c0 >> 6) & ~3;
            c1 = tc16[taddr1 & 0x3ff];
            taddr1 = ((c1 >> 6) & ~3) + 1;
            c2 = tc16[taddr2 & 0x3ff];
            taddr2 = ((c2 >> 6) & ~3) + 2;
            c3 = tc16[taddr3 & 0x3ff];
            taddr3 = (c3 >> 6) | 3;
        }
        break;
    case 15:
    default:
        {
            s1 = s0 + (sdiff << 1);
            taddr0 = (tbase0 << 3) + s0;
            taddr1 = (tbase0 << 3) + s1;
            taddr2 = (tbase2 << 3) + s0;
            taddr3 = (tbase2 << 3) + s1;

            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;
            taddr1 ^= xort;
            xort = (t1 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr2 ^= xort;
            taddr3 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];
            taddr0 = c0 << 2;
            c2 = state[wid].tmem[taddr2 & 0x7ff];
            taddr2 = (c2 << 2) + 2;
            c1 = state[wid].tmem[taddr1 & 0x7ff];
            taddr1 = (c1 << 2) + 1;
            c3 = state[wid].tmem[taddr3 & 0x7ff];
            taddr3 = (c3 << 2) + 3;
        }
        break;
    }

    c0 = tlut[taddr0 ^ xorupperrg];
    c2 = tlut[taddr2 ^ xorupperrg];
    c1 = tlut[taddr1 ^ xorupperrg];
    c3 = tlut[taddr3 ^ xorupperrg];

    if (!state[wid].other_modes.tlut_type)
    {
        color0->r = GET_HI_RGBA16_TMEM(c0);
        color0->g = GET_MED_RGBA16_TMEM(c0);
        color1->r = GET_HI_RGBA16_TMEM(c1);
        color1->g = GET_MED_RGBA16_TMEM(c1);
        color2->r = GET_HI_RGBA16_TMEM(c2);
        color2->g = GET_MED_RGBA16_TMEM(c2);
        color3->r = GET_HI_RGBA16_TMEM(c3);
        color3->g = GET_MED_RGBA16_TMEM(c3);

        if (isupper == isupperrg)
        {
            color0->b = GET_LOW_RGBA16_TMEM(c0);
            color0->a = (c0 & 1) ? 0xff : 0;
            color1->b = GET_LOW_RGBA16_TMEM(c1);
            color1->a = (c1 & 1) ? 0xff : 0;
            color2->b = GET_LOW_RGBA16_TMEM(c2);
            color2->a = (c2 & 1) ? 0xff : 0;
            color3->b = GET_LOW_RGBA16_TMEM(c3);
            color3->a = (c3 & 1) ? 0xff : 0;
        }
        else
        {
            color0->b = GET_LOW_RGBA16_TMEM(c3);
            color0->a = (c3 & 1) ? 0xff : 0;
            color1->b = GET_LOW_RGBA16_TMEM(c2);
            color1->a = (c2 & 1) ? 0xff : 0;
            color2->b = GET_LOW_RGBA16_TMEM(c1);
            color2->a = (c1 & 1) ? 0xff : 0;
            color3->b = GET_LOW_RGBA16_TMEM(c0);
            color3->a = (c0 & 1) ? 0xff : 0;
        }
    }
    else
    {
        color0->r = color0->g = c0 >> 8;
        color1->r = color1->g = c1 >> 8;
        color2->r = color2->g = c2 >> 8;
        color3->r = color3->g = c3 >> 8;

        if (isupper == isupperrg)
        {
            color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            color3->b = c3 >> 8;
            color3->a = c3 & 0xff;
        }
        else
        {
            color0->b = c3 >> 8;
            color0->a = c3 & 0xff;
            color1->b = c2 >> 8;
            color1->a = c2 & 0xff;
            color2->b = c1 >> 8;
            color2->a = c1 & 0xff;
            color3->b = c0 >> 8;
            color3->a = c0 & 0xff;
        }
    }
}

static INLINE void fetch_texel_entlut_quadro_nearest(uint32_t wid, struct color *color0, struct color *color1, struct color *color2, struct color *color3, int s0, int t0, uint32_t tilenum, int isupper, int isupperrg)
{
    uint32_t tbase0 = state[wid].tile[tilenum].line * t0 + state[wid].tile[tilenum].tmem;
    uint32_t tpal = state[wid].tile[tilenum].palette << 4;
    uint32_t xort, ands;

    uint32_t taddr0 = 0;
    uint16_t c0, c1, c2, c3;

    uint32_t xorupperrg = isupperrg ? (WORD_ADDR_XOR ^ 3) : WORD_ADDR_XOR;

    switch(state[wid].tile[tilenum].f.tlutswitch)
    {
    case 0:
    case 1:
    case 2:
        {
            taddr0 = ((tbase0 << 4) + s0) >> 1;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            ands = s0 & 1;
            c0 = state[wid].tmem[taddr0 & 0x7ff];
            c0 = (ands) ? (c0 & 0xf) : (c0 >> 4);

            taddr0 = (tpal | c0) << 2;
        }
        break;
    case 3:
        {
            taddr0 = (tbase0 << 3) + s0;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];

            c0 >>= 4;

            taddr0 = (tpal | c0) << 2;
        }
        break;
    case 4:
    case 5:
    case 6:
        {
            taddr0 = (tbase0 << 3) + s0;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];

            taddr0 = c0 << 2;
        }
        break;
    case 7:
        {
            taddr0 = (tbase0 << 3) + s0;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];

            taddr0 = c0 << 2;
        }
        break;
    case 8:
    case 9:
    case 10:
        {
            taddr0 = (tbase0 << 2) + s0;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;

            c0 = tc16[taddr0 & 0x3ff];

            taddr0 = (c0 >> 6) & ~3;
        }
        break;
    case 11:
        {
            taddr0 = (tbase0 << 3) + s0;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];

            taddr0 = c0 << 2;
        }
        break;
    case 12:
    case 13:
    case 14:
        {
            taddr0 = (tbase0 << 2) + s0;
            xort = (t0 & 1) ? WORD_XOR_DWORD_SWAP : WORD_ADDR_XOR;
            taddr0 ^= xort;

            c0 = tc16[taddr0 & 0x3ff];

            taddr0 = (c0 >> 6) & ~3;
        }
        break;
    case 15:
    default:
        {
            taddr0 = (tbase0 << 3) + s0;
            xort = (t0 & 1) ? BYTE_XOR_DWORD_SWAP : BYTE_ADDR_XOR;
            taddr0 ^= xort;

            c0 = state[wid].tmem[taddr0 & 0x7ff];

            taddr0 = c0 << 2;
        }
        break;
    }

    c0 = tlut[taddr0 ^ xorupperrg];
    c1 = tlut[(taddr0 + 1) ^ xorupperrg];
    c2 = tlut[(taddr0 + 2) ^ xorupperrg];
    c3 = tlut[(taddr0 + 3) ^ xorupperrg];

    if (!state[wid].other_modes.tlut_type)
    {
        color0->r = GET_HI_RGBA16_TMEM(c0);
        color0->g = GET_MED_RGBA16_TMEM(c0);
        color1->r = GET_HI_RGBA16_TMEM(c1);
        color1->g = GET_MED_RGBA16_TMEM(c1);
        color2->r = GET_HI_RGBA16_TMEM(c2);
        color2->g = GET_MED_RGBA16_TMEM(c2);
        color3->r = GET_HI_RGBA16_TMEM(c3);
        color3->g = GET_MED_RGBA16_TMEM(c3);

        if (isupper == isupperrg)
        {
            color0->b = GET_LOW_RGBA16_TMEM(c0);
            color0->a = (c0 & 1) ? 0xff : 0;
            color1->b = GET_LOW_RGBA16_TMEM(c1);
            color1->a = (c1 & 1) ? 0xff : 0;
            color2->b = GET_LOW_RGBA16_TMEM(c2);
            color2->a = (c2 & 1) ? 0xff : 0;
            color3->b = GET_LOW_RGBA16_TMEM(c3);
            color3->a = (c3 & 1) ? 0xff : 0;
        }
        else
        {
            color0->b = GET_LOW_RGBA16_TMEM(c3);
            color0->a = (c3 & 1) ? 0xff : 0;
            color1->b = GET_LOW_RGBA16_TMEM(c2);
            color1->a = (c2 & 1) ? 0xff : 0;
            color2->b = GET_LOW_RGBA16_TMEM(c1);
            color2->a = (c1 & 1) ? 0xff : 0;
            color3->b = GET_LOW_RGBA16_TMEM(c0);
            color3->a = (c0 & 1) ? 0xff : 0;
        }
    }
    else
    {
        color0->r = color0->g = c0 >> 8;
        color1->r = color1->g = c1 >> 8;
        color2->r = color2->g = c2 >> 8;
        color3->r = color3->g = c3 >> 8;

        if (isupper == isupperrg)
        {
            color0->b = c0 >> 8;
            color0->a = c0 & 0xff;
            color1->b = c1 >> 8;
            color1->a = c1 & 0xff;
            color2->b = c2 >> 8;
            color2->a = c2 & 0xff;
            color3->b = c3 >> 8;
            color3->a = c3 & 0xff;
        }
        else
        {
            color0->b = c3 >> 8;
            color0->a = c3 & 0xff;
            color1->b = c2 >> 8;
            color1->a = c2 & 0xff;
            color2->b = c1 >> 8;
            color2->a = c1 & 0xff;
            color3->b = c0 >> 8;
            color3->a = c0 & 0xff;
        }
    }
}

static void get_tmem_idx(uint32_t wid, int s, int t, uint32_t tilenum, uint32_t* idx0, uint32_t* idx1, uint32_t* idx2, uint32_t* idx3, uint32_t* bit3flipped, uint32_t* hibit)
{
    uint32_t tbase = (state[wid].tile[tilenum].line * t) & 0x1ff;
    tbase += state[wid].tile[tilenum].tmem;
    uint32_t tsize = state[wid].tile[tilenum].size;
    uint32_t tformat = state[wid].tile[tilenum].format;
    uint32_t sshorts = 0;


    if (tsize == PIXEL_SIZE_8BIT || tformat == FORMAT_YUV)
        sshorts = s >> 1;
    else if (tsize >= PIXEL_SIZE_16BIT)
        sshorts = s;
    else
        sshorts = s >> 2;
    sshorts &= 0x7ff;

    *bit3flipped = ((sshorts & 2) != 0) ^ (t & 1);

    int tidx_a = ((tbase << 2) + sshorts) & 0x7fd;
    int tidx_b = (tidx_a + 1) & 0x7ff;
    int tidx_c = (tidx_a + 2) & 0x7ff;
    int tidx_d = (tidx_a + 3) & 0x7ff;

    *hibit = (tidx_a & 0x400) != 0;

    if (t & 1)
    {
        tidx_a ^= 2;
        tidx_b ^= 2;
        tidx_c ^= 2;
        tidx_d ^= 2;
    }


    sort_tmem_idx(idx0, tidx_a, tidx_b, tidx_c, tidx_d, 0);
    sort_tmem_idx(idx1, tidx_a, tidx_b, tidx_c, tidx_d, 1);
    sort_tmem_idx(idx2, tidx_a, tidx_b, tidx_c, tidx_d, 2);
    sort_tmem_idx(idx3, tidx_a, tidx_b, tidx_c, tidx_d, 3);
}

static void read_tmem_copy(uint32_t wid, int s, int s1, int s2, int s3, int t, uint32_t tilenum, uint32_t* sortshort, int* hibits, int* lowbits)
{
    uint32_t tbase = (state[wid].tile[tilenum].line * t) & 0x1ff;
    tbase += state[wid].tile[tilenum].tmem;
    uint32_t tsize = state[wid].tile[tilenum].size;
    uint32_t tformat = state[wid].tile[tilenum].format;
    uint32_t shbytes = 0, shbytes1 = 0, shbytes2 = 0, shbytes3 = 0;
    int32_t delta = 0;
    uint32_t sortidx[8];


    if (tsize == PIXEL_SIZE_8BIT || tformat == FORMAT_YUV)
    {
        shbytes = s << 1;
        shbytes1 = s1 << 1;
        shbytes2 = s2 << 1;
        shbytes3 = s3 << 1;
    }
    else if (tsize >= PIXEL_SIZE_16BIT)
    {
        shbytes = s << 2;
        shbytes1 = s1 << 2;
        shbytes2 = s2 << 2;
        shbytes3 = s3 << 2;
    }
    else
    {
        shbytes = s;
        shbytes1 = s1;
        shbytes2 = s2;
        shbytes3 = s3;
    }

    shbytes &= 0x1fff;
    shbytes1 &= 0x1fff;
    shbytes2 &= 0x1fff;
    shbytes3 &= 0x1fff;

    int tidx_a, tidx_blow, tidx_bhi, tidx_c, tidx_dlow, tidx_dhi;

    tbase <<= 4;
    tidx_a = (tbase + shbytes) & 0x1fff;
    tidx_bhi = (tbase + shbytes1) & 0x1fff;
    tidx_c = (tbase + shbytes2) & 0x1fff;
    tidx_dhi = (tbase + shbytes3) & 0x1fff;

    if (tformat == FORMAT_YUV)
    {
        delta = shbytes1 - shbytes;
        tidx_blow = (tidx_a + (delta << 1)) & 0x1fff;
        tidx_dlow = (tidx_blow + shbytes3 - shbytes) & 0x1fff;
    }
    else
    {
        tidx_blow = tidx_bhi;
        tidx_dlow = tidx_dhi;
    }

    if (t & 1)
    {
        tidx_a ^= 8;
        tidx_blow ^= 8;
        tidx_bhi ^= 8;
        tidx_c ^= 8;
        tidx_dlow ^= 8;
        tidx_dhi ^= 8;
    }

    hibits[0] = (tidx_a & 0x1000) != 0;
    hibits[1] = (tidx_blow & 0x1000) != 0;
    hibits[2] = (tidx_bhi & 0x1000) != 0;
    hibits[3] = (tidx_c & 0x1000) != 0;
    hibits[4] = (tidx_dlow & 0x1000) != 0;
    hibits[5] = (tidx_dhi & 0x1000) != 0;
    lowbits[0] = tidx_a & 0xf;
    lowbits[1] = tidx_blow & 0xf;
    lowbits[2] = tidx_bhi & 0xf;
    lowbits[3] = tidx_c & 0xf;
    lowbits[4] = tidx_dlow & 0xf;
    lowbits[5] = tidx_dhi & 0xf;

    uint32_t short0, short1, short2, short3;


    tidx_a >>= 2;
    tidx_blow >>= 2;
    tidx_bhi >>= 2;
    tidx_c >>= 2;
    tidx_dlow >>= 2;
    tidx_dhi >>= 2;


    sort_tmem_idx(&sortidx[0], tidx_a, tidx_blow, tidx_c, tidx_dlow, 0);
    sort_tmem_idx(&sortidx[1], tidx_a, tidx_blow, tidx_c, tidx_dlow, 1);
    sort_tmem_idx(&sortidx[2], tidx_a, tidx_blow, tidx_c, tidx_dlow, 2);
    sort_tmem_idx(&sortidx[3], tidx_a, tidx_blow, tidx_c, tidx_dlow, 3);

    short0 = tmem16[sortidx[0] ^ WORD_ADDR_XOR];
    short1 = tmem16[sortidx[1] ^ WORD_ADDR_XOR];
    short2 = tmem16[sortidx[2] ^ WORD_ADDR_XOR];
    short3 = tmem16[sortidx[3] ^ WORD_ADDR_XOR];


    sort_tmem_shorts_lowhalf(&sortshort[0], short0, short1, short2, short3, lowbits[0] >> 2);
    sort_tmem_shorts_lowhalf(&sortshort[1], short0, short1, short2, short3, lowbits[1] >> 2);
    sort_tmem_shorts_lowhalf(&sortshort[2], short0, short1, short2, short3, lowbits[3] >> 2);
    sort_tmem_shorts_lowhalf(&sortshort[3], short0, short1, short2, short3, lowbits[4] >> 2);

    if (state[wid].other_modes.en_tlut)
    {

        compute_color_index(wid, &short0, sortshort[0], lowbits[0] & 3, tilenum);
        compute_color_index(wid, &short1, sortshort[1], lowbits[1] & 3, tilenum);
        compute_color_index(wid, &short2, sortshort[2], lowbits[3] & 3, tilenum);
        compute_color_index(wid, &short3, sortshort[3], lowbits[4] & 3, tilenum);


        sortidx[4] = (short0 << 2);
        sortidx[5] = (short1 << 2) | 1;
        sortidx[6] = (short2 << 2) | 2;
        sortidx[7] = (short3 << 2) | 3;
    }
    else
    {
        sort_tmem_idx(&sortidx[4], tidx_a, tidx_bhi, tidx_c, tidx_dhi, 0);
        sort_tmem_idx(&sortidx[5], tidx_a, tidx_bhi, tidx_c, tidx_dhi, 1);
        sort_tmem_idx(&sortidx[6], tidx_a, tidx_bhi, tidx_c, tidx_dhi, 2);
        sort_tmem_idx(&sortidx[7], tidx_a, tidx_bhi, tidx_c, tidx_dhi, 3);
    }

    short0 = tmem16[(sortidx[4] | 0x400) ^ WORD_ADDR_XOR];
    short1 = tmem16[(sortidx[5] | 0x400) ^ WORD_ADDR_XOR];
    short2 = tmem16[(sortidx[6] | 0x400) ^ WORD_ADDR_XOR];
    short3 = tmem16[(sortidx[7] | 0x400) ^ WORD_ADDR_XOR];



    if (state[wid].other_modes.en_tlut)
    {
        sort_tmem_shorts_lowhalf(&sortshort[4], short0, short1, short2, short3, 0);
        sort_tmem_shorts_lowhalf(&sortshort[5], short0, short1, short2, short3, 1);
        sort_tmem_shorts_lowhalf(&sortshort[6], short0, short1, short2, short3, 2);
        sort_tmem_shorts_lowhalf(&sortshort[7], short0, short1, short2, short3, 3);
    }
    else
    {
        sort_tmem_shorts_lowhalf(&sortshort[4], short0, short1, short2, short3, lowbits[0] >> 2);
        sort_tmem_shorts_lowhalf(&sortshort[5], short0, short1, short2, short3, lowbits[2] >> 2);
        sort_tmem_shorts_lowhalf(&sortshort[6], short0, short1, short2, short3, lowbits[3] >> 2);
        sort_tmem_shorts_lowhalf(&sortshort[7], short0, short1, short2, short3, lowbits[5] >> 2);
    }
}

static void tmem_init_lut(void)
{
    int i;
    for (i = 0; i < 32; i++)
        replicated_rgba[i] = (i << 3) | ((i >> 2) & 7);
}
