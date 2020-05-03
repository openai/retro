static int32_t blenderone = 0xff;

static uint8_t bldiv_hwaccurate_table[0x8000];

static INLINE void set_blender_input(uint32_t wid, int cycle, int which, int32_t **input_r, int32_t **input_g, int32_t **input_b, int32_t **input_a, int a, int b)
{

    switch (a & 0x3)
    {
        case 0:
        {
            if (cycle == 0)
            {
                *input_r = &state[wid].pixel_color.r;
                *input_g = &state[wid].pixel_color.g;
                *input_b = &state[wid].pixel_color.b;
            }
            else
            {
                *input_r = &state[wid].blended_pixel_color.r;
                *input_g = &state[wid].blended_pixel_color.g;
                *input_b = &state[wid].blended_pixel_color.b;
            }
            break;
        }

        case 1:
        {
            *input_r = &state[wid].memory_color.r;
            *input_g = &state[wid].memory_color.g;
            *input_b = &state[wid].memory_color.b;
            break;
        }

        case 2:
        {
            *input_r = &state[wid].blend_color.r;      *input_g = &state[wid].blend_color.g;      *input_b = &state[wid].blend_color.b;
            break;
        }

        case 3:
        {
            *input_r = &state[wid].fog_color.r;        *input_g = &state[wid].fog_color.g;        *input_b = &state[wid].fog_color.b;
            break;
        }
    }

    if (which == 0)
    {
        switch (b & 0x3)
        {
            case 0:     *input_a = &state[wid].pixel_color.a; break;
            case 1:     *input_a = &state[wid].fog_color.a; break;
            case 2:     *input_a = &state[wid].blender_shade_alpha; break;
            case 3:     *input_a = &zero_color; break;
        }
    }
    else
    {
        switch (b & 0x3)
        {
            case 0:     *input_a = &state[wid].inv_pixel_color.a; break;
            case 1:     *input_a = &state[wid].memory_color.a; break;
            case 2:     *input_a = &blenderone; break;
            case 3:     *input_a = &zero_color; break;
        }
    }
}

static STRICTINLINE int alpha_compare(uint32_t wid, int32_t comb_alpha)
{
    int32_t threshold;
    if (!state[wid].other_modes.alpha_compare_en)
        return 1;
    else
    {
        if (!state[wid].other_modes.dither_alpha_en)
            threshold = state[wid].blend_color.a;
        else
            threshold = irand(&state[wid].rseed) & 0xff;


        if (comb_alpha >= threshold)
            return 1;
        else
            return 0;
    }
}

static STRICTINLINE void blender_equation_cycle0(uint32_t wid, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    int blr, blg, blb, sum;
    blend1a = *state[wid].blender1b_a[0] >> 3;
    blend2a = *state[wid].blender2b_a[0] >> 3;

    int mulb;



    if (state[wid].blender2b_a[0] == &state[wid].memory_color.a)
    {
        blend1a = (blend1a >> state[wid].blshifta) & 0x3C;
        blend2a = (blend2a >> state[wid].blshiftb) | 3;
    }

    mulb = blend2a + 1;


    blr = (*state[wid].blender1a_r[0]) * blend1a + (*state[wid].blender2a_r[0]) * mulb;
    blg = (*state[wid].blender1a_g[0]) * blend1a + (*state[wid].blender2a_g[0]) * mulb;
    blb = (*state[wid].blender1a_b[0]) * blend1a + (*state[wid].blender2a_b[0]) * mulb;



    if (!state[wid].other_modes.force_blend)
    {





        sum = ((blend1a & ~3) + (blend2a & ~3) + 4) << 9;
        *r = bldiv_hwaccurate_table[sum | ((blr >> 2) & 0x7ff)];
        *g = bldiv_hwaccurate_table[sum | ((blg >> 2) & 0x7ff)];
        *b = bldiv_hwaccurate_table[sum | ((blb >> 2) & 0x7ff)];
    }
    else
    {
        *r = (blr >> 5) & 0xff;
        *g = (blg >> 5) & 0xff;
        *b = (blb >> 5) & 0xff;
    }
}

static STRICTINLINE void blender_equation_cycle0_2(uint32_t wid, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    blend1a = *state[wid].blender1b_a[0] >> 3;
    blend2a = *state[wid].blender2b_a[0] >> 3;

    if (state[wid].blender2b_a[0] == &state[wid].memory_color.a)
    {
        blend1a = (blend1a >> state[wid].pastblshifta) & 0x3C;
        blend2a = (blend2a >> state[wid].pastblshiftb) | 3;
    }

    blend2a += 1;
    *r = (((*state[wid].blender1a_r[0]) * blend1a + (*state[wid].blender2a_r[0]) * blend2a) >> 5) & 0xff;
    *g = (((*state[wid].blender1a_g[0]) * blend1a + (*state[wid].blender2a_g[0]) * blend2a) >> 5) & 0xff;
    *b = (((*state[wid].blender1a_b[0]) * blend1a + (*state[wid].blender2a_b[0]) * blend2a) >> 5) & 0xff;
}

static STRICTINLINE void blender_equation_cycle1(uint32_t wid, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    int blr, blg, blb, sum;
    blend1a = *state[wid].blender1b_a[1] >> 3;
    blend2a = *state[wid].blender2b_a[1] >> 3;

    int mulb;
    if (state[wid].blender2b_a[1] == &state[wid].memory_color.a)
    {
        blend1a = (blend1a >> state[wid].blshifta) & 0x3C;
        blend2a = (blend2a >> state[wid].blshiftb) | 3;
    }

    mulb = blend2a + 1;
    blr = (*state[wid].blender1a_r[1]) * blend1a + (*state[wid].blender2a_r[1]) * mulb;
    blg = (*state[wid].blender1a_g[1]) * blend1a + (*state[wid].blender2a_g[1]) * mulb;
    blb = (*state[wid].blender1a_b[1]) * blend1a + (*state[wid].blender2a_b[1]) * mulb;

    if (!state[wid].other_modes.force_blend)
    {
        sum = ((blend1a & ~3) + (blend2a & ~3) + 4) << 9;
        *r = bldiv_hwaccurate_table[sum | ((blr >> 2) & 0x7ff)];
        *g = bldiv_hwaccurate_table[sum | ((blg >> 2) & 0x7ff)];
        *b = bldiv_hwaccurate_table[sum | ((blb >> 2) & 0x7ff)];
    }
    else
    {
        *r = (blr >> 5) & 0xff;
        *g = (blg >> 5) & 0xff;
        *b = (blb >> 5) & 0xff;
    }
}

static STRICTINLINE int blender_1cycle(uint32_t wid, uint32_t* fr, uint32_t* fg, uint32_t* fb, int dith, uint32_t blend_en, uint32_t prewrap, uint32_t curpixel_cvg, uint32_t curpixel_cvbit)
{
    int r, g, b, dontblend;


    if (alpha_compare(wid, state[wid].pixel_color.a))
    {






        if (state[wid].other_modes.antialias_en ? curpixel_cvg : curpixel_cvbit)
        {

            if (!state[wid].other_modes.color_on_cvg || prewrap)
            {
                dontblend = (state[wid].other_modes.f.partialreject_1cycle && state[wid].pixel_color.a >= 0xff);
                if (!blend_en || dontblend)
                {
                    r = *state[wid].blender1a_r[0];
                    g = *state[wid].blender1a_g[0];
                    b = *state[wid].blender1a_b[0];
                }
                else
                {
                    state[wid].inv_pixel_color.a =  (~(*state[wid].blender1b_a[0])) & 0xff;





                    blender_equation_cycle0(wid, &r, &g, &b);
                }
            }
            else
            {
                r = *state[wid].blender2a_r[0];
                g = *state[wid].blender2a_g[0];
                b = *state[wid].blender2a_b[0];
            }

            if (state[wid].other_modes.rgb_dither_sel != 3)
                rgb_dither(state[wid].other_modes.rgb_dither_sel, &r, &g, &b, dith);

            *fr = r;
            *fg = g;
            *fb = b;
            return 1;
        }
        else
            return 0;
        }
    else
        return 0;
}

static STRICTINLINE int blender_2cycle_cycle0(uint32_t wid, uint32_t curpixel_cvg, uint32_t curpixel_cvbit)
{
    int r, g, b;
    int wen = (state[wid].other_modes.antialias_en ? curpixel_cvg : curpixel_cvbit) > 0 ? 1 : 0;

    if (wen)
    {
        state[wid].inv_pixel_color.a =  (~(*state[wid].blender1b_a[0])) & 0xff;

        blender_equation_cycle0_2(wid, &r, &g, &b);

        state[wid].blended_pixel_color.r = r;
        state[wid].blended_pixel_color.g = g;
        state[wid].blended_pixel_color.b = b;
    }

    state[wid].memory_color = state[wid].pre_memory_color;

    return wen;
}


static STRICTINLINE void blender_2cycle_cycle1(uint32_t wid, uint32_t* fr, uint32_t* fg, uint32_t* fb, int dith, uint32_t blend_en, uint32_t prewrap)
{
    int r, g, b, dontblend;

    if (!state[wid].other_modes.color_on_cvg || prewrap)
    {
        dontblend = (state[wid].other_modes.f.partialreject_2cycle && state[wid].pixel_color.a >= 0xff);
        if (!blend_en || dontblend)
        {
            r = *state[wid].blender1a_r[1];
            g = *state[wid].blender1a_g[1];
            b = *state[wid].blender1a_b[1];
        }
        else
        {
            state[wid].inv_pixel_color.a =  (~(*state[wid].blender1b_a[1])) & 0xff;
            blender_equation_cycle1(wid, &r, &g, &b);
        }
    }
    else
    {
        r = *state[wid].blender2a_r[1];
        g = *state[wid].blender2a_g[1];
        b = *state[wid].blender2a_b[1];
    }

    if (state[wid].other_modes.rgb_dither_sel != 3)
        rgb_dither(state[wid].other_modes.rgb_dither_sel, &r, &g, &b, dith);

    *fr = r;
    *fg = g;
    *fb = b;
}

static void blender_init_lut(void)
{
    int i, k;
    int d = 0, n = 0, temp = 0, res = 0, invd = 0, nbit = 0;
    int ps[9];
    for (i = 0; i < 0x8000; i++)
    {
        res = 0;
        d = (i >> 11) & 0xf;
        n = i & 0x7ff;
        invd = (~d) & 0xf;


        temp = invd + (n >> 8) + 1;
        ps[0] = temp & 7;
        for (k = 0; k < 8; k++)
        {
            nbit = (n >> (7 - k)) & 1;
            if (res & (0x100 >> k))
                temp = invd + (ps[k] << 1) + nbit + 1;
            else
                temp = d + (ps[k] << 1) + nbit;
            ps[k + 1] = temp & 7;
            if (temp & 0x10)
                res |= (1 << (7 - k));
        }
        bldiv_hwaccurate_table[i] = res;
    }
}

void rdp_set_fog_color(uint32_t wid, const uint32_t* args)
{
    state[wid].fog_color.r = RGBA32_R(args[1]);
    state[wid].fog_color.g = RGBA32_G(args[1]);
    state[wid].fog_color.b = RGBA32_B(args[1]);
    state[wid].fog_color.a = RGBA32_A(args[1]);
}

void rdp_set_blend_color(uint32_t wid, const uint32_t* args)
{
    state[wid].blend_color.r = RGBA32_R(args[1]);
    state[wid].blend_color.g = RGBA32_G(args[1]);
    state[wid].blend_color.b = RGBA32_B(args[1]);
    state[wid].blend_color.a = RGBA32_A(args[1]);
}
