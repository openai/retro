static uint8_t gamma_table[0x100];
static uint8_t gamma_dither_table[0x4000];

static uint32_t vi_integer_sqrt(uint32_t a)
{
    unsigned long op = a, res = 0, one = 1 << 30;

    while (one > op)
        one >>= 2;

    while (one != 0)
    {
        if (op >= res + one)
        {
            op -= res + one;
            res += one << 1;
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

static STRICTINLINE void gamma_filters(struct rgba* pixel, bool gamma_enable, bool gamma_dither_enable, uint32_t* rstate)
{
    int cdith, dith;

    switch((gamma_enable << 1) | gamma_dither_enable)
    {
    case 0: // no gamma, no dithering
        return;
    case 1: // no gamma, dithering enabled
        cdith = irand(rstate);
        dith = cdith & 1;
        if (pixel->r < 255)
            pixel->r += dith;
        dith = (cdith >> 1) & 1;
        if (pixel->g < 255)
            pixel->g += dith;
        dith = (cdith >> 2) & 1;
        if (pixel->b < 255)
            pixel->b += dith;
        break;
    case 2: // gamma enabled, no dithering
        pixel->r = gamma_table[pixel->r];
        pixel->g = gamma_table[pixel->g];
        pixel->b = gamma_table[pixel->b];
        break;
    case 3: // gamma and dithering enabled
        cdith = irand(rstate);
        dith = cdith & 0x3f;
        pixel->r = gamma_dither_table[((pixel->r) << 6)|dith];
        dith = (cdith >> 6) & 0x3f;
        pixel->g = gamma_dither_table[((pixel->g) << 6)|dith];
        dith = ((cdith >> 9) & 0x38) | (cdith & 7);
        pixel->b = gamma_dither_table[((pixel->b) << 6)|dith];
        break;
    }
}

void vi_gamma_init(void)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        gamma_table[i] = vi_integer_sqrt(i << 6);
        gamma_table[i] <<= 1;
    }

    for (i = 0; i < 0x4000; i++)
    {
        gamma_dither_table[i] = vi_integer_sqrt(i);
        gamma_dither_table[i] <<= 1;
    }
}
