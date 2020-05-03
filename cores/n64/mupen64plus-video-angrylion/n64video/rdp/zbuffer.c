#define ZMODE_OPAQUE            0
#define ZMODE_INTERPENETRATING  1
#define ZMODE_TRANSPARENT       2
#define ZMODE_DECAL             3

static uint16_t z_com_table[0x40000];
static uint32_t z_complete_dec_table[0x4000];
static uint16_t deltaz_comparator_lut[0x10000];

static struct {uint32_t shift; uint32_t add;} z_dec_table[8] = {
     6, 0x00000,
     5, 0x20000,
     4, 0x30000,
     3, 0x38000,
     2, 0x3c000,
     1, 0x3e000,
     0, 0x3f000,
     0, 0x3f800,
};

static STRICTINLINE uint32_t z_decompress(uint32_t zb)
{
    return z_complete_dec_table[(zb >> 2) & 0x3fff];
}

static INLINE void z_build_com_table(void)
{

    int z;
    uint16_t altmem = 0;
    for(z = 0; z < 0x40000; z++)
    {
    switch((z >> 11) & 0x7f)
    {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
        altmem = (z >> 4) & 0x1ffc;
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
    case 0x5f:
        altmem = ((z >> 3) & 0x1ffc) | 0x2000;
        break;
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x69:
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
    case 0x6e:
    case 0x6f:
        altmem = ((z >> 2) & 0x1ffc) | 0x4000;
        break;
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
        altmem = ((z >> 1) & 0x1ffc) | 0x6000;
        break;
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
        altmem = (z & 0x1ffc) | 0x8000;
        break;
    case 0x7c:
    case 0x7d:
        altmem = ((z << 1) & 0x1ffc) | 0xa000;
        break;
    case 0x7e:
        altmem = ((z << 2) & 0x1ffc) | 0xc000;
        break;
    case 0x7f:
        altmem = ((z << 2) & 0x1ffc) | 0xe000;
        break;
    default:
        msg_error("z_build_com_table failed");
        break;
    }

    z_com_table[z] = altmem;

    }
}

static STRICTINLINE void z_store(uint32_t zcurpixel, uint32_t z, int dzpixenc)
{
    uint16_t zval = z_com_table[z & 0x3ffff]|(dzpixenc >> 2);
    uint8_t hval = dzpixenc & 3;
    PAIRWRITE16(zcurpixel, zval, hval);
}

static STRICTINLINE uint32_t dz_decompress(uint32_t dz_compressed)
{
    return (1 << dz_compressed);
}


static STRICTINLINE uint32_t dz_compress(uint32_t value)
{
    int j = 0;
    if (value & 0xff00)
        j |= 8;
    if (value & 0xf0f0)
        j |= 4;
    if (value & 0xcccc)
        j |= 2;
    if (value & 0xaaaa)
        j |= 1;
    return j;
}

static STRICTINLINE uint32_t z_compare(uint32_t wid, uint32_t zcurpixel, uint32_t sz, uint16_t dzpix, int dzpixenc, uint32_t* blend_en, uint32_t* prewrap, uint32_t* curpixel_cvg, uint32_t curpixel_memcvg)
{


    int force_coplanar = 0;
    sz &= 0x3ffff;

    uint8_t hval;
    uint16_t zval;
    uint32_t oz, dzmem;
    int32_t rawdzmem;

    if (state[wid].other_modes.z_compare_en)
    {
        PAIRREAD16(zval, hval, zcurpixel);
        oz = z_decompress(zval);
        rawdzmem = ((zval & 3) << 2) | hval;
        dzmem = dz_decompress(rawdzmem);



        if (state[wid].other_modes.f.realblendershiftersneeded)
        {
            state[wid].blshifta = clamp(dzpixenc - rawdzmem, 0, 4);
            state[wid].blshiftb = clamp(rawdzmem - dzpixenc, 0, 4);

        }


        if (state[wid].other_modes.f.interpixelblendershiftersneeded)
        {
            state[wid].pastblshifta = clamp(dzpixenc - state[wid].pastrawdzmem, 0, 4);
            state[wid].pastblshiftb = clamp(state[wid].pastrawdzmem - dzpixenc, 0, 4);
        }

        state[wid].pastrawdzmem = rawdzmem;

        int precision_factor = (zval >> 13) & 0xf;




        uint32_t dzmemmodifier;
        if (precision_factor < 3)
        {
            if (dzmem != 0x8000)
            {
                dzmemmodifier = 16 >> precision_factor;
                dzmem <<= 1;
                if (dzmem < dzmemmodifier)
                    dzmem = dzmemmodifier;

            }
            else
            {
                force_coplanar = 1;
                dzmem = 0xffff;
            }
        }






        uint32_t dznew = (uint32_t)deltaz_comparator_lut[dzpix | dzmem];

        uint32_t dznotshift = dznew;
        dznew <<= 3;


        uint32_t farther = force_coplanar || ((sz + dznew) >= oz);

        int overflow = (curpixel_memcvg + *curpixel_cvg) & 8;
        *blend_en = state[wid].other_modes.force_blend || (!overflow && state[wid].other_modes.antialias_en && farther);

        *prewrap = overflow;



        int cvgcoeff = 0;
        uint32_t dzenc = 0;

        int32_t diff;
        uint32_t nearer, max, infront;

        switch(state[wid].other_modes.z_mode)
        {
        case ZMODE_OPAQUE:
            infront = sz < oz;
            diff = (int32_t)sz - (int32_t)dznew;
            nearer = force_coplanar || (diff <= (int32_t)oz);
            max = (oz == 0x3ffff);
            return (max || (overflow ? infront : nearer));
            break;
        case ZMODE_INTERPENETRATING:
            infront = sz < oz;
            if (!infront || !farther || !overflow)
            {
                diff = (int32_t)sz - (int32_t)dznew;
                nearer = force_coplanar || (diff <= (int32_t)oz);
                max = (oz == 0x3ffff);
                return (max || (overflow ? infront : nearer));
            }
            else
            {
                dzenc = dz_compress(dznotshift & 0xffff);
                cvgcoeff = ((oz >> dzenc) - (sz >> dzenc)) & 0xf;
                *curpixel_cvg = ((cvgcoeff * (*curpixel_cvg)) >> 3) & 0xf;
                return 1;
            }
            break;
        case ZMODE_TRANSPARENT:
            infront = sz < oz;
            max = (oz == 0x3ffff);
            return (infront || max);
            break;
        case ZMODE_DECAL:
            diff = (int32_t)sz - (int32_t)dznew;
            nearer = force_coplanar || (diff <= (int32_t)oz);
            max = (oz == 0x3ffff);
            return (farther && nearer && !max);
            break;
        }
        return 0;
    }
    else
    {


        if (state[wid].other_modes.f.realblendershiftersneeded)
        {
            state[wid].blshifta = 0;
            if (dzpixenc < 0xb)
                state[wid].blshiftb = 4;
            else
                state[wid].blshiftb = 0xf - dzpixenc;
        }

        if (state[wid].other_modes.f.interpixelblendershiftersneeded)
        {
            state[wid].pastblshifta = 0;
            if (dzpixenc < 0xb)
                state[wid].pastblshiftb = 4;
            else
                state[wid].pastblshiftb = 0xf - dzpixenc;
        }

        state[wid].pastrawdzmem = 0xf;

        int overflow = (curpixel_memcvg + *curpixel_cvg) & 8;
        *blend_en = state[wid].other_modes.force_blend || (!overflow && state[wid].other_modes.antialias_en);
        *prewrap = overflow;

        return 1;
    }
}

void rdp_set_mask_image(uint32_t wid, const uint32_t* args)
{
    state[wid].zb_address  = args[1] & 0x0ffffff;
}

void z_init_lut(void)
{
    int i;
    z_build_com_table();

    uint32_t exponent;
    uint32_t mantissa;
    for (i = 0; i < 0x4000; i++)
    {
        exponent = (i >> 11) & 7;
        mantissa = i & 0x7ff;
        z_complete_dec_table[i] = ((mantissa << z_dec_table[exponent].shift) + z_dec_table[exponent].add) & 0x3ffff;
    }

    deltaz_comparator_lut[0] = 0;
    for (i = 1; i < 0x10000; i++)
    {
       int k;
        for (k = 15; k >= 0; k--)
        {
            if (i & (1 << k))
            {
                deltaz_comparator_lut[i] = 1 << k;
                break;
            }
        }
    }
}
