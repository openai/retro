static int vi_restore_table[0x400];

static STRICTINLINE void restore_filter16(int* r, int* g, int* b, uint32_t fboffset, uint32_t num, uint32_t hres, uint32_t fetchbugstate)
{
    int i;
    uint32_t idx = (fboffset >> 1) + num;

    uint32_t toleftpix = idx - 1;

    uint32_t leftuppix, leftdownpix, maxpix;

    leftuppix = idx - hres - 1;

    if (fetchbugstate != 1)
    {
        leftdownpix = idx + hres - 1;
        maxpix = idx + hres + 1;
    }
    else
    {
        leftdownpix = toleftpix;
        maxpix = toleftpix + 2;
    }

    int rend = *r;
    int gend = *g;
    int bend = *b;
    const int* redptr = &vi_restore_table[(rend << 2) & 0x3e0];
    const int* greenptr = &vi_restore_table[(gend << 2) & 0x3e0];
    const int* blueptr = &vi_restore_table[(bend << 2) & 0x3e0];

    uint32_t tempr, tempg, tempb;
    uint16_t pix;

    const uint32_t dirs[] =
    {
        leftuppix, leftuppix + 1, leftuppix + 2, leftdownpix,
        leftdownpix + 1, maxpix, toleftpix, toleftpix + 2
    };

    if (rdram_valid_idx16(maxpix) && rdram_valid_idx16(leftuppix))
    {
        for (i = 0; i < 8; i++)
        {
            pix = rdram_read_idx16_fast(dirs[i]);
            tempr = (pix >> 11) & 0x1f;
            tempg = (pix >> 6) & 0x1f;
            tempb = (pix >> 1) & 0x1f;
            rend += redptr[tempr];
            gend += greenptr[tempg];
            bend += blueptr[tempb];
        }
    }
    else
    {
        for (i = 0; i < 8; i++)
        {
            pix = rdram_read_idx16(dirs[i]);
            tempr = (pix >> 11) & 0x1f;
            tempg = (pix >> 6) & 0x1f;
            tempb = (pix >> 1) & 0x1f;
            rend += redptr[tempr];
            gend += greenptr[tempg];
            bend += blueptr[tempb];
        }
    }


    *r = rend;
    *g = gend;
    *b = bend;
}

static STRICTINLINE void restore_filter32(int* r, int* g, int* b, uint32_t fboffset, uint32_t num, uint32_t hres, uint32_t fetchbugstate)
{
    int i;
    uint32_t idx = (fboffset >> 2) + num;

    uint32_t toleftpix = idx - 1;

    uint32_t leftuppix, leftdownpix, maxpix;

    leftuppix = idx - hres - 1;

    if (fetchbugstate != 1)
    {
        leftdownpix = idx + hres - 1;
        maxpix = idx +hres + 1;
    }
    else
    {
        leftdownpix = toleftpix;
        maxpix = toleftpix + 2;
    }

    int rend = *r;
    int gend = *g;
    int bend = *b;
    const int* redptr = &vi_restore_table[(rend << 2) & 0x3e0];
    const int* greenptr = &vi_restore_table[(gend << 2) & 0x3e0];
    const int* blueptr = &vi_restore_table[(bend << 2) & 0x3e0];

    uint32_t tempr, tempg, tempb;
    uint32_t pix;

    const uint32_t dirs[] =
    {
        leftuppix, leftuppix + 1, leftuppix + 2, leftdownpix,
        leftdownpix + 1, maxpix, toleftpix, toleftpix + 2
    };

    if (rdram_valid_idx32(maxpix) && rdram_valid_idx32(leftuppix))
    {
        for (i = 0; i < 8; i++)
        {
            pix = rdram_read_idx32_fast(dirs[i]);
            tempr = (pix >> 27) & 0x1f;
            tempg = (pix >> 19) & 0x1f;
            tempb = (pix >> 11) & 0x1f;
            rend += redptr[tempr];
            gend += greenptr[tempg];
            bend += blueptr[tempb];
        }
    }
    else
    {
        for (i = 0; i < 8; i++)
        {
            pix = rdram_read_idx32(dirs[i]);
            tempr = (pix >> 27) & 0x1f;
            tempg = (pix >> 19) & 0x1f;
            tempb = (pix >> 11) & 0x1f;
            rend += redptr[tempr];
            gend += greenptr[tempg];
            bend += blueptr[tempb];
        }
    }

    *r = rend;
    *g = gend;
    *b = bend;
}

void vi_restore_init()
{
    int i;
    for (i = 0; i < 0x400; i++)
    {
        if (((i >> 5) & 0x1f) < (i & 0x1f))
            vi_restore_table[i] = 1;
        else if (((i >> 5) & 0x1f) > (i & 0x1f))
            vi_restore_table[i] = -1;
        else
            vi_restore_table[i] = 0;
    }
}
