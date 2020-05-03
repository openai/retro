static uint32_t special_9bit_clamptable[512];
static int32_t special_9bit_exttable[512];

static INLINE void set_suba_rgb_input(uint32_t wid, int32_t **input_r, int32_t **input_g, int32_t **input_b, int code)
{
    switch (code & 0xf)
    {
        case 0:     *input_r = &state[wid].combined_color.r;   *input_g = &state[wid].combined_color.g;   *input_b = &state[wid].combined_color.b;   break;
        case 1:     *input_r = &state[wid].texel0_color.r;     *input_g = &state[wid].texel0_color.g;     *input_b = &state[wid].texel0_color.b;     break;
        case 2:     *input_r = &state[wid].texel1_color.r;     *input_g = &state[wid].texel1_color.g;     *input_b = &state[wid].texel1_color.b;     break;
        case 3:     *input_r = &state[wid].prim_color.r;       *input_g = &state[wid].prim_color.g;       *input_b = &state[wid].prim_color.b;       break;
        case 4:     *input_r = &state[wid].shade_color.r;      *input_g = &state[wid].shade_color.g;      *input_b = &state[wid].shade_color.b;      break;
        case 5:     *input_r = &state[wid].env_color.r;        *input_g = &state[wid].env_color.g;        *input_b = &state[wid].env_color.b;        break;
        case 6:     *input_r = &one_color;          *input_g = &one_color;          *input_b = &one_color;      break;
        case 7:     *input_r = &state[wid].noise;              *input_g = &state[wid].noise;              *input_b = &state[wid].noise;              break;
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        {
            *input_r = &zero_color;     *input_g = &zero_color;     *input_b = &zero_color;     break;
        }
    }
}

static INLINE void set_subb_rgb_input(uint32_t wid, int32_t **input_r, int32_t **input_g, int32_t **input_b, int code)
{
    switch (code & 0xf)
    {
        case 0:     *input_r = &state[wid].combined_color.r;   *input_g = &state[wid].combined_color.g;   *input_b = &state[wid].combined_color.b;   break;
        case 1:     *input_r = &state[wid].texel0_color.r;     *input_g = &state[wid].texel0_color.g;     *input_b = &state[wid].texel0_color.b;     break;
        case 2:     *input_r = &state[wid].texel1_color.r;     *input_g = &state[wid].texel1_color.g;     *input_b = &state[wid].texel1_color.b;     break;
        case 3:     *input_r = &state[wid].prim_color.r;       *input_g = &state[wid].prim_color.g;       *input_b = &state[wid].prim_color.b;       break;
        case 4:     *input_r = &state[wid].shade_color.r;      *input_g = &state[wid].shade_color.g;      *input_b = &state[wid].shade_color.b;      break;
        case 5:     *input_r = &state[wid].env_color.r;        *input_g = &state[wid].env_color.g;        *input_b = &state[wid].env_color.b;        break;
        case 6:     *input_r = &state[wid].key_center.r;       *input_g = &state[wid].key_center.g;       *input_b = &state[wid].key_center.b;       break;
        case 7:     *input_r = &state[wid].k4;                 *input_g = &state[wid].k4;                 *input_b = &state[wid].k4;                 break;
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        {
            *input_r = &zero_color;     *input_g = &zero_color;     *input_b = &zero_color;     break;
        }
    }
}

static INLINE void set_mul_rgb_input(uint32_t wid, int32_t **input_r, int32_t **input_g, int32_t **input_b, int code)
{
    switch (code & 0x1f)
    {
        case 0:     *input_r = &state[wid].combined_color.r;   *input_g = &state[wid].combined_color.g;   *input_b = &state[wid].combined_color.b;   break;
        case 1:     *input_r = &state[wid].texel0_color.r;     *input_g = &state[wid].texel0_color.g;     *input_b = &state[wid].texel0_color.b;     break;
        case 2:     *input_r = &state[wid].texel1_color.r;     *input_g = &state[wid].texel1_color.g;     *input_b = &state[wid].texel1_color.b;     break;
        case 3:     *input_r = &state[wid].prim_color.r;       *input_g = &state[wid].prim_color.g;       *input_b = &state[wid].prim_color.b;       break;
        case 4:     *input_r = &state[wid].shade_color.r;      *input_g = &state[wid].shade_color.g;      *input_b = &state[wid].shade_color.b;      break;
        case 5:     *input_r = &state[wid].env_color.r;        *input_g = &state[wid].env_color.g;        *input_b = &state[wid].env_color.b;        break;
        case 6:     *input_r = &state[wid].key_scale.r;        *input_g = &state[wid].key_scale.g;        *input_b = &state[wid].key_scale.b;        break;
        case 7:     *input_r = &state[wid].combined_color.a;   *input_g = &state[wid].combined_color.a;   *input_b = &state[wid].combined_color.a;   break;
        case 8:     *input_r = &state[wid].texel0_color.a;     *input_g = &state[wid].texel0_color.a;     *input_b = &state[wid].texel0_color.a;     break;
        case 9:     *input_r = &state[wid].texel1_color.a;     *input_g = &state[wid].texel1_color.a;     *input_b = &state[wid].texel1_color.a;     break;
        case 10:    *input_r = &state[wid].prim_color.a;       *input_g = &state[wid].prim_color.a;       *input_b = &state[wid].prim_color.a;       break;
        case 11:    *input_r = &state[wid].shade_color.a;      *input_g = &state[wid].shade_color.a;      *input_b = &state[wid].shade_color.a;      break;
        case 12:    *input_r = &state[wid].env_color.a;        *input_g = &state[wid].env_color.a;        *input_b = &state[wid].env_color.a;        break;
        case 13:    *input_r = &state[wid].lod_frac;           *input_g = &state[wid].lod_frac;           *input_b = &state[wid].lod_frac;           break;
        case 14:    *input_r = &state[wid].primitive_lod_frac; *input_g = &state[wid].primitive_lod_frac; *input_b = &state[wid].primitive_lod_frac; break;
        case 15:    *input_r = &state[wid].k5;                 *input_g = &state[wid].k5;                 *input_b = &state[wid].k5;                 break;
        case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
        case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
        {
            *input_r = &zero_color;     *input_g = &zero_color;     *input_b = &zero_color;     break;
        }
    }
}

static INLINE void set_add_rgb_input(uint32_t wid, int32_t **input_r, int32_t **input_g, int32_t **input_b, int code)
{
    switch (code & 0x7)
    {
        case 0:     *input_r = &state[wid].combined_color.r;   *input_g = &state[wid].combined_color.g;   *input_b = &state[wid].combined_color.b;   break;
        case 1:     *input_r = &state[wid].texel0_color.r;     *input_g = &state[wid].texel0_color.g;     *input_b = &state[wid].texel0_color.b;     break;
        case 2:     *input_r = &state[wid].texel1_color.r;     *input_g = &state[wid].texel1_color.g;     *input_b = &state[wid].texel1_color.b;     break;
        case 3:     *input_r = &state[wid].prim_color.r;       *input_g = &state[wid].prim_color.g;       *input_b = &state[wid].prim_color.b;       break;
        case 4:     *input_r = &state[wid].shade_color.r;      *input_g = &state[wid].shade_color.g;      *input_b = &state[wid].shade_color.b;      break;
        case 5:     *input_r = &state[wid].env_color.r;        *input_g = &state[wid].env_color.g;        *input_b = &state[wid].env_color.b;        break;
        case 6:     *input_r = &one_color;          *input_g = &one_color;          *input_b = &one_color;          break;
        case 7:     *input_r = &zero_color;         *input_g = &zero_color;         *input_b = &zero_color;         break;
    }
}

static INLINE void set_sub_alpha_input(uint32_t wid, int32_t **input, int code)
{
    switch (code & 0x7)
    {
        case 0:     *input = &state[wid].combined_color.a; break;
        case 1:     *input = &state[wid].texel0_color.a; break;
        case 2:     *input = &state[wid].texel1_color.a; break;
        case 3:     *input = &state[wid].prim_color.a; break;
        case 4:     *input = &state[wid].shade_color.a; break;
        case 5:     *input = &state[wid].env_color.a; break;
        case 6:     *input = &one_color; break;
        case 7:     *input = &zero_color; break;
    }
}

static INLINE void set_mul_alpha_input(uint32_t wid, int32_t **input, int code)
{
    switch (code & 0x7)
    {
        case 0:     *input = &state[wid].lod_frac; break;
        case 1:     *input = &state[wid].texel0_color.a; break;
        case 2:     *input = &state[wid].texel1_color.a; break;
        case 3:     *input = &state[wid].prim_color.a; break;
        case 4:     *input = &state[wid].shade_color.a; break;
        case 5:     *input = &state[wid].env_color.a; break;
        case 6:     *input = &state[wid].primitive_lod_frac; break;
        case 7:     *input = &zero_color; break;
    }
}

static STRICTINLINE int32_t color_combiner_equation(int32_t a, int32_t b, int32_t c, int32_t d)
{





    a = special_9bit_exttable[a];
    b = special_9bit_exttable[b];
    c = SIGNF(c, 9);
    d = special_9bit_exttable[d];
    a = ((a - b) * c) + (d << 8) + 0x80;
    return (a & 0x1ffff);
}

static STRICTINLINE int32_t alpha_combiner_equation(int32_t a, int32_t b, int32_t c, int32_t d)
{
    a = special_9bit_exttable[a];
    b = special_9bit_exttable[b];
    c = SIGNF(c, 9);
    d = special_9bit_exttable[d];
    a = (((a - b) * c) + (d << 8) + 0x80) >> 8;
    return (a & 0x1ff);
}

static STRICTINLINE int32_t chroma_key_min(uint32_t wid, struct color* col)
{
    int32_t redkey, greenkey, bluekey, keyalpha;




    redkey = SIGN(col->r, 17);
    if (redkey > 0)
        redkey = ((redkey & 0xf) == 8) ? (-redkey + 0x10) : (-redkey);

    redkey = (state[wid].key_width.r << 4) + redkey;

    greenkey = SIGN(col->g, 17);
    if (greenkey > 0)
        greenkey = ((greenkey & 0xf) == 8) ? (-greenkey + 0x10) : (-greenkey);

    greenkey = (state[wid].key_width.g << 4) + greenkey;

    bluekey = SIGN(col->b, 17);
    if (bluekey > 0)
        bluekey = ((bluekey & 0xf) == 8) ? (-bluekey + 0x10) : (-bluekey);

    bluekey = (state[wid].key_width.b << 4) + bluekey;

    keyalpha = (redkey < greenkey) ? redkey : greenkey;
    keyalpha = (bluekey < keyalpha) ? bluekey : keyalpha;
    keyalpha = clamp(keyalpha, 0, 0xff);
    return keyalpha;
}

static STRICTINLINE void combiner_1cycle(uint32_t wid, int adseed, uint32_t* curpixel_cvg)
{

    int32_t keyalpha, temp;
    struct color chromabypass;

    if (state[wid].other_modes.key_en)
    {
        chromabypass.r = *state[wid].combiner_rgbsub_a_r[1];
        chromabypass.g = *state[wid].combiner_rgbsub_a_g[1];
        chromabypass.b = *state[wid].combiner_rgbsub_a_b[1];
    }






    if (state[wid].combiner_rgbmul_r[1] != &zero_color)
    {
















        state[wid].combined_color.r = color_combiner_equation(*state[wid].combiner_rgbsub_a_r[1],*state[wid].combiner_rgbsub_b_r[1],*state[wid].combiner_rgbmul_r[1],*state[wid].combiner_rgbadd_r[1]);
        state[wid].combined_color.g = color_combiner_equation(*state[wid].combiner_rgbsub_a_g[1],*state[wid].combiner_rgbsub_b_g[1],*state[wid].combiner_rgbmul_g[1],*state[wid].combiner_rgbadd_g[1]);
        state[wid].combined_color.b = color_combiner_equation(*state[wid].combiner_rgbsub_a_b[1],*state[wid].combiner_rgbsub_b_b[1],*state[wid].combiner_rgbmul_b[1],*state[wid].combiner_rgbadd_b[1]);
    }
    else
    {
        state[wid].combined_color.r = ((special_9bit_exttable[*state[wid].combiner_rgbadd_r[1]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.g = ((special_9bit_exttable[*state[wid].combiner_rgbadd_g[1]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.b = ((special_9bit_exttable[*state[wid].combiner_rgbadd_b[1]] << 8) + 0x80) & 0x1ffff;
    }

    if (state[wid].combiner_alphamul[1] != &zero_color)
        state[wid].combined_color.a = alpha_combiner_equation(*state[wid].combiner_alphasub_a[1],*state[wid].combiner_alphasub_b[1],*state[wid].combiner_alphamul[1],*state[wid].combiner_alphaadd[1]);
    else
        state[wid].combined_color.a = special_9bit_exttable[*state[wid].combiner_alphaadd[1]] & 0x1ff;

    state[wid].pixel_color.a = special_9bit_clamptable[state[wid].combined_color.a];
    if (state[wid].pixel_color.a == 0xff)
        state[wid].pixel_color.a = 0x100;

    if (!state[wid].other_modes.key_en)
    {

        state[wid].combined_color.r >>= 8;
        state[wid].combined_color.g >>= 8;
        state[wid].combined_color.b >>= 8;
        state[wid].pixel_color.r = special_9bit_clamptable[state[wid].combined_color.r];
        state[wid].pixel_color.g = special_9bit_clamptable[state[wid].combined_color.g];
        state[wid].pixel_color.b = special_9bit_clamptable[state[wid].combined_color.b];
    }
    else
    {
        keyalpha = chroma_key_min(wid, &state[wid].combined_color);



        state[wid].pixel_color.r = special_9bit_clamptable[chromabypass.r];
        state[wid].pixel_color.g = special_9bit_clamptable[chromabypass.g];
        state[wid].pixel_color.b = special_9bit_clamptable[chromabypass.b];


        state[wid].combined_color.r >>= 8;
        state[wid].combined_color.g >>= 8;
        state[wid].combined_color.b >>= 8;
    }


    if (state[wid].other_modes.cvg_times_alpha)
    {
        temp = (state[wid].pixel_color.a * (*curpixel_cvg) + 4) >> 3;
        *curpixel_cvg = (temp >> 5) & 0xf;
    }

    if (!state[wid].other_modes.alpha_cvg_select)
    {
        if (!state[wid].other_modes.key_en)
        {
            state[wid].pixel_color.a += adseed;
            if (state[wid].pixel_color.a & 0x100)
                state[wid].pixel_color.a = 0xff;
        }
        else
            state[wid].pixel_color.a = keyalpha;
    }
    else
    {
        if (state[wid].other_modes.cvg_times_alpha)
            state[wid].pixel_color.a = temp;
        else
            state[wid].pixel_color.a = (*curpixel_cvg) << 5;
        if (state[wid].pixel_color.a > 0xff)
            state[wid].pixel_color.a = 0xff;
    }

    state[wid].blender_shade_alpha = state[wid].shade_color.a + adseed;
    if (state[wid].blender_shade_alpha & 0x100)
        state[wid].blender_shade_alpha = 0xff;
}

static STRICTINLINE void combiner_2cycle_cycle0(uint32_t wid, int adseed, uint32_t cvg, uint32_t* acalpha)
{
    if (state[wid].combiner_rgbmul_r[0] != &zero_color)
    {
        state[wid].combined_color.r = color_combiner_equation(*state[wid].combiner_rgbsub_a_r[0],*state[wid].combiner_rgbsub_b_r[0],*state[wid].combiner_rgbmul_r[0],*state[wid].combiner_rgbadd_r[0]);
        state[wid].combined_color.g = color_combiner_equation(*state[wid].combiner_rgbsub_a_g[0],*state[wid].combiner_rgbsub_b_g[0],*state[wid].combiner_rgbmul_g[0],*state[wid].combiner_rgbadd_g[0]);
        state[wid].combined_color.b = color_combiner_equation(*state[wid].combiner_rgbsub_a_b[0],*state[wid].combiner_rgbsub_b_b[0],*state[wid].combiner_rgbmul_b[0],*state[wid].combiner_rgbadd_b[0]);
    }
    else
    {
        state[wid].combined_color.r = ((special_9bit_exttable[*state[wid].combiner_rgbadd_r[0]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.g = ((special_9bit_exttable[*state[wid].combiner_rgbadd_g[0]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.b = ((special_9bit_exttable[*state[wid].combiner_rgbadd_b[0]] << 8) + 0x80) & 0x1ffff;
    }

    if (state[wid].combiner_alphamul[0] != &zero_color)
        state[wid].combined_color.a = alpha_combiner_equation(*state[wid].combiner_alphasub_a[0],*state[wid].combiner_alphasub_b[0],*state[wid].combiner_alphamul[0],*state[wid].combiner_alphaadd[0]);
    else
        state[wid].combined_color.a = special_9bit_exttable[*state[wid].combiner_alphaadd[0]] & 0x1ff;



    if (state[wid].other_modes.alpha_compare_en)
    {
        int32_t preacalpha = special_9bit_clamptable[state[wid].combined_color.a];
        if (preacalpha == 0xff)
            preacalpha = 0x100;

        if (!state[wid].other_modes.alpha_cvg_select)
        {
            preacalpha += adseed;
            if (preacalpha & 0x100)
                preacalpha = 0xff;
        }
        else
        {
            if (state[wid].other_modes.cvg_times_alpha)
                preacalpha = (preacalpha * cvg + 4) >> 3;
            else
                preacalpha = cvg << 5;

            if (preacalpha > 0xff)
                preacalpha = 0xff;
        }

        *acalpha = preacalpha;
    }





    state[wid].combined_color.r >>= 8;
    state[wid].combined_color.g >>= 8;
    state[wid].combined_color.b >>= 8;

    state[wid].blender_shade_alpha = state[wid].shade_color.a + adseed;
    if (state[wid].blender_shade_alpha & 0x100)
        state[wid].blender_shade_alpha = 0xff;
}

static STRICTINLINE void combiner_2cycle_cycle1(uint32_t wid, int adseed, uint32_t* curpixel_cvg)
{
    int32_t keyalpha, temp;
    struct color chromabypass;

    state[wid].texel0_color = state[wid].texel1_color;
    state[wid].texel1_color = state[wid].nexttexel_color;









    if (state[wid].other_modes.key_en)
    {
        chromabypass.r = *state[wid].combiner_rgbsub_a_r[1];
        chromabypass.g = *state[wid].combiner_rgbsub_a_g[1];
        chromabypass.b = *state[wid].combiner_rgbsub_a_b[1];
    }

    if (state[wid].combiner_rgbmul_r[1] != &zero_color)
    {
        state[wid].combined_color.r = color_combiner_equation(*state[wid].combiner_rgbsub_a_r[1],*state[wid].combiner_rgbsub_b_r[1],*state[wid].combiner_rgbmul_r[1],*state[wid].combiner_rgbadd_r[1]);
        state[wid].combined_color.g = color_combiner_equation(*state[wid].combiner_rgbsub_a_g[1],*state[wid].combiner_rgbsub_b_g[1],*state[wid].combiner_rgbmul_g[1],*state[wid].combiner_rgbadd_g[1]);
        state[wid].combined_color.b = color_combiner_equation(*state[wid].combiner_rgbsub_a_b[1],*state[wid].combiner_rgbsub_b_b[1],*state[wid].combiner_rgbmul_b[1],*state[wid].combiner_rgbadd_b[1]);
    }
    else
    {
        state[wid].combined_color.r = ((special_9bit_exttable[*state[wid].combiner_rgbadd_r[1]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.g = ((special_9bit_exttable[*state[wid].combiner_rgbadd_g[1]] << 8) + 0x80) & 0x1ffff;
        state[wid].combined_color.b = ((special_9bit_exttable[*state[wid].combiner_rgbadd_b[1]] << 8) + 0x80) & 0x1ffff;
    }

    if (state[wid].combiner_alphamul[1] != &zero_color)
        state[wid].combined_color.a = alpha_combiner_equation(*state[wid].combiner_alphasub_a[1],*state[wid].combiner_alphasub_b[1],*state[wid].combiner_alphamul[1],*state[wid].combiner_alphaadd[1]);
    else
        state[wid].combined_color.a = special_9bit_exttable[*state[wid].combiner_alphaadd[1]] & 0x1ff;

    if (!state[wid].other_modes.key_en)
    {

        state[wid].combined_color.r >>= 8;
        state[wid].combined_color.g >>= 8;
        state[wid].combined_color.b >>= 8;

        state[wid].pixel_color.r = special_9bit_clamptable[state[wid].combined_color.r];
        state[wid].pixel_color.g = special_9bit_clamptable[state[wid].combined_color.g];
        state[wid].pixel_color.b = special_9bit_clamptable[state[wid].combined_color.b];
    }
    else
    {
        keyalpha = chroma_key_min(wid, &state[wid].combined_color);



        state[wid].pixel_color.r = special_9bit_clamptable[chromabypass.r];
        state[wid].pixel_color.g = special_9bit_clamptable[chromabypass.g];
        state[wid].pixel_color.b = special_9bit_clamptable[chromabypass.b];


        state[wid].combined_color.r >>= 8;
        state[wid].combined_color.g >>= 8;
        state[wid].combined_color.b >>= 8;
    }

    state[wid].pixel_color.a = special_9bit_clamptable[state[wid].combined_color.a];
    if (state[wid].pixel_color.a == 0xff)
        state[wid].pixel_color.a = 0x100;


    if (state[wid].other_modes.cvg_times_alpha)
    {
        temp = (state[wid].pixel_color.a * (*curpixel_cvg) + 4) >> 3;

        *curpixel_cvg = (temp >> 5) & 0xf;


    }

    if (!state[wid].other_modes.alpha_cvg_select)
    {
        if (!state[wid].other_modes.key_en)
        {
            state[wid].pixel_color.a += adseed;
            if (state[wid].pixel_color.a & 0x100)
                state[wid].pixel_color.a = 0xff;
        }
        else
            state[wid].pixel_color.a = keyalpha;
    }
    else
    {
        if (state[wid].other_modes.cvg_times_alpha)
            state[wid].pixel_color.a = temp;
        else
            state[wid].pixel_color.a = (*curpixel_cvg) << 5;
        if (state[wid].pixel_color.a > 0xff)
            state[wid].pixel_color.a = 0xff;
    }

    state[wid].blender_shade_alpha = state[wid].shade_color.a + adseed;
    if (state[wid].blender_shade_alpha & 0x100)
        state[wid].blender_shade_alpha = 0xff;
}

static void combiner_init_lut(void)
{
    int i;
    for(i = 0; i < 0x200; i++)
    {
        switch((i >> 7) & 3)
        {
        case 0:
        case 1:
            special_9bit_clamptable[i] = i & 0xff;
            break;
        case 2:
            special_9bit_clamptable[i] = 0xff;
            break;
        case 3:
            special_9bit_clamptable[i] = 0;
            break;
        }
    }

    for (i = 0; i < 0x200; i++)
    {
        special_9bit_exttable[i] = ((i & 0x180) == 0x180) ? (i | ~0x1ff) : (i & 0x1ff);
    }
}

static void combiner_init(uint32_t wid)
{
    state[wid].combiner_rgbsub_a_r[0] = state[wid].combiner_rgbsub_a_r[1] = &one_color;
    state[wid].combiner_rgbsub_a_g[0] = state[wid].combiner_rgbsub_a_g[1] = &one_color;
    state[wid].combiner_rgbsub_a_b[0] = state[wid].combiner_rgbsub_a_b[1] = &one_color;
    state[wid].combiner_rgbsub_b_r[0] = state[wid].combiner_rgbsub_b_r[1] = &one_color;
    state[wid].combiner_rgbsub_b_g[0] = state[wid].combiner_rgbsub_b_g[1] = &one_color;
    state[wid].combiner_rgbsub_b_b[0] = state[wid].combiner_rgbsub_b_b[1] = &one_color;
    state[wid].combiner_rgbmul_r[0] = state[wid].combiner_rgbmul_r[1] = &one_color;
    state[wid].combiner_rgbmul_g[0] = state[wid].combiner_rgbmul_g[1] = &one_color;
    state[wid].combiner_rgbmul_b[0] = state[wid].combiner_rgbmul_b[1] = &one_color;
    state[wid].combiner_rgbadd_r[0] = state[wid].combiner_rgbadd_r[1] = &one_color;
    state[wid].combiner_rgbadd_g[0] = state[wid].combiner_rgbadd_g[1] = &one_color;
    state[wid].combiner_rgbadd_b[0] = state[wid].combiner_rgbadd_b[1] = &one_color;

    state[wid].combiner_alphasub_a[0] = state[wid].combiner_alphasub_a[1] = &one_color;
    state[wid].combiner_alphasub_b[0] = state[wid].combiner_alphasub_b[1] = &one_color;
    state[wid].combiner_alphamul[0] = state[wid].combiner_alphamul[1] = &one_color;
    state[wid].combiner_alphaadd[0] = state[wid].combiner_alphaadd[1] = &one_color;
}

void rdp_set_prim_color(uint32_t wid, const uint32_t* args)
{
    state[wid].min_level = (args[0] >> 8) & 0x1f;
    state[wid].primitive_lod_frac = args[0] & 0xff;
    state[wid].prim_color.r = RGBA32_R(args[1]);
    state[wid].prim_color.g = RGBA32_G(args[1]);
    state[wid].prim_color.b = RGBA32_B(args[1]);
    state[wid].prim_color.a = RGBA32_A(args[1]);
}

void rdp_set_env_color(uint32_t wid, const uint32_t* args)
{
    state[wid].env_color.r = RGBA32_R(args[1]);
    state[wid].env_color.g = RGBA32_G(args[1]);
    state[wid].env_color.b = RGBA32_B(args[1]);
    state[wid].env_color.a = RGBA32_A(args[1]);
}

void rdp_set_combine(uint32_t wid, const uint32_t* args)
{
    state[wid].combine.sub_a_rgb0  = (args[0] >> 20) & 0xf;
    state[wid].combine.mul_rgb0    = (args[0] >> 15) & 0x1f;
    state[wid].combine.sub_a_a0    = (args[0] >> 12) & 0x7;
    state[wid].combine.mul_a0      = (args[0] >>  9) & 0x7;
    state[wid].combine.sub_a_rgb1  = (args[0] >>  5) & 0xf;
    state[wid].combine.mul_rgb1    = (args[0] >>  0) & 0x1f;

    state[wid].combine.sub_b_rgb0  = (args[1] >> 28) & 0xf;
    state[wid].combine.sub_b_rgb1  = (args[1] >> 24) & 0xf;
    state[wid].combine.sub_a_a1    = (args[1] >> 21) & 0x7;
    state[wid].combine.mul_a1      = (args[1] >> 18) & 0x7;
    state[wid].combine.add_rgb0    = (args[1] >> 15) & 0x7;
    state[wid].combine.sub_b_a0    = (args[1] >> 12) & 0x7;
    state[wid].combine.add_a0      = (args[1] >>  9) & 0x7;
    state[wid].combine.add_rgb1    = (args[1] >>  6) & 0x7;
    state[wid].combine.sub_b_a1    = (args[1] >>  3) & 0x7;
    state[wid].combine.add_a1      = (args[1] >>  0) & 0x7;


    set_suba_rgb_input(wid, &state[wid].combiner_rgbsub_a_r[0], &state[wid].combiner_rgbsub_a_g[0], &state[wid].combiner_rgbsub_a_b[0], state[wid].combine.sub_a_rgb0);
    set_subb_rgb_input(wid, &state[wid].combiner_rgbsub_b_r[0], &state[wid].combiner_rgbsub_b_g[0], &state[wid].combiner_rgbsub_b_b[0], state[wid].combine.sub_b_rgb0);
    set_mul_rgb_input(wid, &state[wid].combiner_rgbmul_r[0], &state[wid].combiner_rgbmul_g[0], &state[wid].combiner_rgbmul_b[0], state[wid].combine.mul_rgb0);
    set_add_rgb_input(wid, &state[wid].combiner_rgbadd_r[0], &state[wid].combiner_rgbadd_g[0], &state[wid].combiner_rgbadd_b[0], state[wid].combine.add_rgb0);
    set_sub_alpha_input(wid, &state[wid].combiner_alphasub_a[0], state[wid].combine.sub_a_a0);
    set_sub_alpha_input(wid, &state[wid].combiner_alphasub_b[0], state[wid].combine.sub_b_a0);
    set_mul_alpha_input(wid, &state[wid].combiner_alphamul[0], state[wid].combine.mul_a0);
    set_sub_alpha_input(wid, &state[wid].combiner_alphaadd[0], state[wid].combine.add_a0);

    set_suba_rgb_input(wid, &state[wid].combiner_rgbsub_a_r[1], &state[wid].combiner_rgbsub_a_g[1], &state[wid].combiner_rgbsub_a_b[1], state[wid].combine.sub_a_rgb1);
    set_subb_rgb_input(wid, &state[wid].combiner_rgbsub_b_r[1], &state[wid].combiner_rgbsub_b_g[1], &state[wid].combiner_rgbsub_b_b[1], state[wid].combine.sub_b_rgb1);
    set_mul_rgb_input(wid, &state[wid].combiner_rgbmul_r[1], &state[wid].combiner_rgbmul_g[1], &state[wid].combiner_rgbmul_b[1], state[wid].combine.mul_rgb1);
    set_add_rgb_input(wid, &state[wid].combiner_rgbadd_r[1], &state[wid].combiner_rgbadd_g[1], &state[wid].combiner_rgbadd_b[1], state[wid].combine.add_rgb1);
    set_sub_alpha_input(wid, &state[wid].combiner_alphasub_a[1], state[wid].combine.sub_a_a1);
    set_sub_alpha_input(wid, &state[wid].combiner_alphasub_b[1], state[wid].combine.sub_b_a1);
    set_mul_alpha_input(wid, &state[wid].combiner_alphamul[1], state[wid].combine.mul_a1);
    set_sub_alpha_input(wid, &state[wid].combiner_alphaadd[1], state[wid].combine.add_a1);

    state[wid].other_modes.f.stalederivs = 1;
}

void rdp_set_key_gb(uint32_t wid, const uint32_t* args)
{
    state[wid].key_width.g = (args[0] >> 12) & 0xfff;
    state[wid].key_width.b = args[0] & 0xfff;
    state[wid].key_center.g = (args[1] >> 24) & 0xff;
    state[wid].key_scale.g = (args[1] >> 16) & 0xff;
    state[wid].key_center.b = (args[1] >> 8) & 0xff;
    state[wid].key_scale.b = args[1] & 0xff;
}

void rdp_set_key_r(uint32_t wid, const uint32_t* args)
{
    state[wid].key_width.r = (args[1] >> 16) & 0xfff;
    state[wid].key_center.r = (args[1] >> 8) & 0xff;
    state[wid].key_scale.r = args[1] & 0xff;
}
