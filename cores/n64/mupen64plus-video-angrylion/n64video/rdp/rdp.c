// bit constants for DP_STATUS
#define DP_STATUS_XBUS_DMA      0x001   // DMEM DMA mode is set
#define DP_STATUS_FREEZE        0x002   // Freeze has been set
#define DP_STATUS_FLUSH         0x004   // Flush has been set
#define DP_STATUS_START_GCLK    0x008   // Unknown
#define DP_STATUS_TMEM_BUSY     0x010   // TMEM is in use on the RDP
#define DP_STATUS_PIPE_BUSY     0x020   // Graphics pipe is in use on the RDP
#define DP_STATUS_CMD_BUSY      0x040   // RDP is currently executing a command
#define DP_STATUS_CBUF_BUSY     0x080   // RDRAM RDP command buffer is in use
#define DP_STATUS_DMA_BUSY      0x100   // DMEM RDP command buffer is in use
#define DP_STATUS_END_VALID     0x200   // Unknown
#define DP_STATUS_START_VALID   0x400   // Unknown

#define PIXEL_SIZE_4BIT         0
#define PIXEL_SIZE_8BIT         1
#define PIXEL_SIZE_16BIT        2
#define PIXEL_SIZE_32BIT        3

#define CYCLE_TYPE_1            0
#define CYCLE_TYPE_2            1
#define CYCLE_TYPE_COPY         2
#define CYCLE_TYPE_FILL         3


#define FORMAT_RGBA             0
#define FORMAT_YUV              1
#define FORMAT_CI               2
#define FORMAT_IA               3
#define FORMAT_I                4


#define TEXEL_RGBA4             0
#define TEXEL_RGBA8             1
#define TEXEL_RGBA16            2
#define TEXEL_RGBA32            3
#define TEXEL_YUV4              4
#define TEXEL_YUV8              5
#define TEXEL_YUV16             6
#define TEXEL_YUV32             7
#define TEXEL_CI4               8
#define TEXEL_CI8               9
#define TEXEL_CI16              0xa
#define TEXEL_CI32              0xb
#define TEXEL_IA4               0xc
#define TEXEL_IA8               0xd
#define TEXEL_IA16              0xe
#define TEXEL_IA32              0xf
#define TEXEL_I4                0x10
#define TEXEL_I8                0x11
#define TEXEL_I16               0x12
#define TEXEL_I32               0x13

struct color
{
    int32_t r, g, b, a;
};

struct rectangle
{
    uint16_t xl, yl, xh, yh;
};

struct other_modes
{
    int cycle_type;
    int persp_tex_en;
    int detail_tex_en;
    int sharpen_tex_en;
    int tex_lod_en;
    int en_tlut;
    int tlut_type;
    int sample_type;
    int mid_texel;
    int bi_lerp0;
    int bi_lerp1;
    int convert_one;
    int key_en;
    int rgb_dither_sel;
    int alpha_dither_sel;
    int blend_m1a_0;
    int blend_m1a_1;
    int blend_m1b_0;
    int blend_m1b_1;
    int blend_m2a_0;
    int blend_m2a_1;
    int blend_m2b_0;
    int blend_m2b_1;
    int force_blend;
    int alpha_cvg_select;
    int cvg_times_alpha;
    int z_mode;
    int cvg_dest;
    int color_on_cvg;
    int image_read_en;
    int z_update_en;
    int z_compare_en;
    int antialias_en;
    int z_source_sel;
    int dither_alpha_en;
    int alpha_compare_en;

    struct
    {
        int stalederivs;
        int dolod;
        int partialreject_1cycle;
        int partialreject_2cycle;
        int rgb_alpha_dither;
        int realblendershiftersneeded;
        int interpixelblendershiftersneeded;
        int getditherlevel;
        int textureuselevel0;
        int textureuselevel1;
    } f;
};

struct spansigs
{
    int endspan;
    int preendspan;
    int nextspan;
    int midspan;
    int longspan;
    int onelessthanmid;
};

struct tile
{
    int format;
    int size;
    int line;
    int tmem;
    int palette;
    int ct, mt, cs, ms;
    int mask_t, shift_t, mask_s, shift_s;

    uint16_t sl, tl, sh, th;

    struct
    {
        int clampdiffs, clampdifft;
        int clampens, clampent;
        int masksclamped, masktclamped;
        int notlutswitch, tlutswitch;
    } f;
};

struct span
{
    int lx, rx;
    int unscrx;
    int validline;
    int32_t r, g, b, a, s, t, w, z;
    int32_t majorx[4];
    int32_t minorx[4];
    int32_t invalyscan[4];
};

struct combiner_inputs
{
    int sub_a_rgb0;
    int sub_b_rgb0;
    int mul_rgb0;
    int add_rgb0;
    int sub_a_a0;
    int sub_b_a0;
    int mul_a0;
    int add_a0;

    int sub_a_rgb1;
    int sub_b_rgb1;
    int mul_rgb1;
    int add_rgb1;
    int sub_a_a1;
    int sub_b_a1;
    int mul_a1;
    int add_a1;
};

struct rdp_state
{
    uint32_t stride;
    uint32_t offset;

    int blshifta;
    int blshiftb;
    int pastblshifta;
    int pastblshiftb;

    struct span span[1024];

    // span states
    int spans_ds;
    int spans_dt;
    int spans_dw;
    int spans_dr;
    int spans_dg;
    int spans_db;
    int spans_da;
    int spans_dz;
    int spans_dzpix;

    int spans_drdy;
    int spans_dgdy;
    int spans_dbdy;
    int spans_dady;
    int spans_dzdy;
    int spans_cdr;
    int spans_cdg;
    int spans_cdb;
    int spans_cda;
    int spans_cdz;

    int spans_dsdy;
    int spans_dtdy;
    int spans_dwdy;

    struct other_modes other_modes;

    struct color combined_color;
    struct color texel0_color;
    struct color texel1_color;
    struct color nexttexel_color;
    struct color shade_color;
    int32_t noise;
    int32_t primitive_lod_frac;

    struct color pixel_color;
    struct color memory_color;
    struct color pre_memory_color;

    struct tile tile[8];

    int32_t k0_tf;
    int32_t k1_tf;
    int32_t k2_tf;
    int32_t k3_tf;
    int32_t k4;
    int32_t k5;
    int32_t lod_frac;

    uint32_t max_level;
    int32_t min_level;

    // irand
    uint32_t rseed;

    // blender
    int32_t *blender1a_r[2];
    int32_t *blender1a_g[2];
    int32_t *blender1a_b[2];
    int32_t *blender1b_a[2];
    int32_t *blender2a_r[2];
    int32_t *blender2a_g[2];
    int32_t *blender2a_b[2];
    int32_t *blender2b_a[2];

    int32_t blender_shade_alpha;

    struct color blend_color;
    struct color fog_color;
    struct color inv_pixel_color;
    struct color blended_pixel_color;

    // combiner
    struct combiner_inputs combine;

    int32_t *combiner_rgbsub_a_r[2];
    int32_t *combiner_rgbsub_a_g[2];
    int32_t *combiner_rgbsub_a_b[2];
    int32_t *combiner_rgbsub_b_r[2];
    int32_t *combiner_rgbsub_b_g[2];
    int32_t *combiner_rgbsub_b_b[2];
    int32_t *combiner_rgbmul_r[2];
    int32_t *combiner_rgbmul_g[2];
    int32_t *combiner_rgbmul_b[2];
    int32_t *combiner_rgbadd_r[2];
    int32_t *combiner_rgbadd_g[2];
    int32_t *combiner_rgbadd_b[2];

    int32_t *combiner_alphasub_a[2];
    int32_t *combiner_alphasub_b[2];
    int32_t *combiner_alphamul[2];
    int32_t *combiner_alphaadd[2];

    struct color prim_color;
    struct color env_color;
    struct color key_scale;
    struct color key_center;
    struct color key_width;

    int32_t keyalpha;

    // tcoord
    void (*tcdiv_ptr)(int32_t, int32_t, int32_t, int32_t*, int32_t*);

    // fbuffer
    void (*fbread1_ptr)(struct rdp_state*, uint32_t, uint32_t*);
    void (*fbread2_ptr)(struct rdp_state*, uint32_t, uint32_t*);
    void (*fbwrite_ptr)(struct rdp_state*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

    int fb_format;
    int fb_size;
    int fb_width;
    uint32_t fb_address;
    uint32_t fill_color;

    // rasterizer
    struct rectangle clip;
    int scfield;
    int sckeepodd;

    uint32_t primitive_z;
    uint16_t primitive_delta_z;

    // tex
    int ti_format;
    int ti_size;
    int ti_width;
    uint32_t ti_address;

    // coverage
    uint8_t cvgbuf[1024];

    // tmem
    uint8_t tmem[0x1000];

    // zbuffer
    uint32_t zb_address;
    int32_t pastrawdzmem;
};

static int32_t one_color = 0x100;
static int32_t zero_color = 0x00;

static const struct
{
    // command handler function pointer
    void (*handler)(struct rdp_state* rdp, const uint32_t*);
    // command data length in bytes
    uint32_t length;
} rdp_commands[] = {
    {rdp_noop,                8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_tri_noshade,        32},
    {rdp_tri_noshade_z,      48},
    {rdp_tri_tex,            96},
    {rdp_tri_tex_z,         112},
    {rdp_tri_shade,          96},
    {rdp_tri_shade_z,       112},
    {rdp_tri_texshade,      160},
    {rdp_tri_texshade_z,    176},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_invalid,             8},
    {rdp_tex_rect,           16},
    {rdp_tex_rect_flip,      16},
    {rdp_sync_load,           8},
    {rdp_sync_pipe,           8},
    {rdp_sync_tile,           8},
    {rdp_sync_full,           8},
    {rdp_set_key_gb,          8},
    {rdp_set_key_r,           8},
    {rdp_set_convert,         8},
    {rdp_set_scissor,         8},
    {rdp_set_prim_depth,      8},
    {rdp_set_other_modes,     8},
    {rdp_load_tlut,           8},
    {rdp_invalid,             8},
    {rdp_set_tile_size,       8},
    {rdp_load_block,          8},
    {rdp_load_tile,           8},
    {rdp_set_tile,            8},
    {rdp_fill_rect,           8},
    {rdp_set_fill_color,      8},
    {rdp_set_fog_color,       8},
    {rdp_set_blend_color,     8},
    {rdp_set_prim_color,      8},
    {rdp_set_env_color,       8},
    {rdp_set_combine,         8},
    {rdp_set_texture_image,   8},
    {rdp_set_mask_image,      8},
    {rdp_set_color_image,     8}
};

static void deduce_derivatives(struct rdp_state* rdp);

#include "rdram.c"
#include "dither.c"
#include "blender.c"
#include "combiner.c"
#include "coverage.c"
#include "zbuffer.c"
#include "fbuffer.c"
#include "tmem.c"
#include "tcoord.c"
#include "tex.c"
#include "rasterizer.c"

static void deduce_derivatives(struct rdp_state* rdp)
{
    int special_bsel0, special_bsel1;


    rdp->other_modes.f.partialreject_1cycle = (rdp->blender2b_a[0] == &rdp->inv_pixel_color.a && rdp->blender1b_a[0] == &rdp->pixel_color.a);
    rdp->other_modes.f.partialreject_2cycle = (rdp->blender2b_a[1] == &rdp->inv_pixel_color.a && rdp->blender1b_a[1] == &rdp->pixel_color.a);


    special_bsel0 = (rdp->blender2b_a[0] == &rdp->memory_color.a);
    special_bsel1 = (rdp->blender2b_a[1] == &rdp->memory_color.a);


    rdp->other_modes.f.realblendershiftersneeded = (special_bsel0 && rdp->other_modes.cycle_type == CYCLE_TYPE_1) || (special_bsel1 && rdp->other_modes.cycle_type == CYCLE_TYPE_2);
    rdp->other_modes.f.interpixelblendershiftersneeded = (special_bsel0 && rdp->other_modes.cycle_type == CYCLE_TYPE_2);

    rdp->other_modes.f.rgb_alpha_dither = (rdp->other_modes.rgb_dither_sel << 2) | rdp->other_modes.alpha_dither_sel;

    rdp->tcdiv_ptr = tcdiv_func[rdp->other_modes.persp_tex_en];


    int texel1_used_in_cc1 = 0, texel0_used_in_cc1 = 0, texel0_used_in_cc0 = 0, texel1_used_in_cc0 = 0;
    int texels_in_cc0 = 0, texels_in_cc1 = 0;
    int lod_frac_used_in_cc1 = 0, lod_frac_used_in_cc0 = 0;
    int texels_or_lf_used_in_ac0 = 0, texel0_used_in_ac0 = 0, texel1_used_in_ac0 = 0;

    if ((rdp->combiner_rgbmul_r[1] == &rdp->lod_frac) || (rdp->combiner_alphamul[1] == &rdp->lod_frac))
        lod_frac_used_in_cc1 = 1;
    if ((rdp->combiner_rgbmul_r[0] == &rdp->lod_frac) || (rdp->combiner_alphamul[0] == &rdp->lod_frac))
        lod_frac_used_in_cc0 = 1;

    if (rdp->combiner_rgbmul_r[1] == &rdp->texel1_color.r || rdp->combiner_rgbsub_a_r[1] == &rdp->texel1_color.r || rdp->combiner_rgbsub_b_r[1] == &rdp->texel1_color.r || rdp->combiner_rgbadd_r[1] == &rdp->texel1_color.r || \
        rdp->combiner_alphamul[1] == &rdp->texel1_color.a || rdp->combiner_alphasub_a[1] == &rdp->texel1_color.a || rdp->combiner_alphasub_b[1] == &rdp->texel1_color.a || rdp->combiner_alphaadd[1] == &rdp->texel1_color.a || \
        rdp->combiner_rgbmul_r[1] == &rdp->texel1_color.a)
        texel1_used_in_cc1 = 1;
    if (rdp->combiner_rgbmul_r[1] == &rdp->texel0_color.r || rdp->combiner_rgbsub_a_r[1] == &rdp->texel0_color.r || rdp->combiner_rgbsub_b_r[1] == &rdp->texel0_color.r || rdp->combiner_rgbadd_r[1] == &rdp->texel0_color.r || \
        rdp->combiner_alphamul[1] == &rdp->texel0_color.a || rdp->combiner_alphasub_a[1] == &rdp->texel0_color.a || rdp->combiner_alphasub_b[1] == &rdp->texel0_color.a || rdp->combiner_alphaadd[1] == &rdp->texel0_color.a || \
        rdp->combiner_rgbmul_r[1] == &rdp->texel0_color.a)
        texel0_used_in_cc1 = 1;
    if (rdp->combiner_alphamul[0] == &rdp->texel1_color.a || rdp->combiner_alphasub_a[0] == &rdp->texel1_color.a || rdp->combiner_alphasub_b[0] == &rdp->texel1_color.a || rdp->combiner_alphaadd[0] == &rdp->texel1_color.a)
        texel1_used_in_ac0 = 1;
    if (rdp->combiner_alphamul[0] == &rdp->texel0_color.a || rdp->combiner_alphasub_a[0] == &rdp->texel0_color.a || rdp->combiner_alphasub_b[0] == &rdp->texel0_color.a || rdp->combiner_alphaadd[0] == &rdp->texel0_color.a)
        texel0_used_in_ac0 = 1;
    if (rdp->combiner_rgbmul_r[0] == &rdp->texel1_color.r || rdp->combiner_rgbsub_a_r[0] == &rdp->texel1_color.r || rdp->combiner_rgbsub_b_r[0] == &rdp->texel1_color.r || rdp->combiner_rgbadd_r[0] == &rdp->texel1_color.r || \
        texel1_used_in_ac0 || rdp->combiner_rgbmul_r[0] == &rdp->texel1_color.a)
        texel1_used_in_cc0 = 1;
    if (rdp->combiner_rgbmul_r[0] == &rdp->texel0_color.r || rdp->combiner_rgbsub_a_r[0] == &rdp->texel0_color.r || rdp->combiner_rgbsub_b_r[0] == &rdp->texel0_color.r || rdp->combiner_rgbadd_r[0] == &rdp->texel0_color.r || \
        texel0_used_in_ac0 || rdp->combiner_rgbmul_r[0] == &rdp->texel0_color.a)
        texel0_used_in_cc0 = 1;
    texels_or_lf_used_in_ac0 = texel0_used_in_ac0 || texel1_used_in_ac0 || (rdp->combiner_alphamul[0] == &rdp->lod_frac);
    texels_in_cc0 = texel0_used_in_cc0 || texel1_used_in_cc0;
    texels_in_cc1 = texel0_used_in_cc1 || texel1_used_in_cc1;


    if (texel1_used_in_cc1)
        rdp->other_modes.f.textureuselevel0 = 0;
    else if (texel0_used_in_cc1 || lod_frac_used_in_cc1)
        rdp->other_modes.f.textureuselevel0 = 1;
    else
        rdp->other_modes.f.textureuselevel0 = 2;

    if (texel1_used_in_cc1 || (rdp->other_modes.alpha_compare_en && texels_or_lf_used_in_ac0))
        rdp->other_modes.f.textureuselevel1 = 0;
    else if (texel1_used_in_cc0 || texel0_used_in_cc1)
        rdp->other_modes.f.textureuselevel1 = 1;
    else if (texel0_used_in_cc0 || lod_frac_used_in_cc0 || lod_frac_used_in_cc1)
        rdp->other_modes.f.textureuselevel1 = 2;
    else
        rdp->other_modes.f.textureuselevel1 = 3;


    int lodfracused = 0;

    if ((rdp->other_modes.cycle_type == CYCLE_TYPE_2 && (lod_frac_used_in_cc0 || lod_frac_used_in_cc1)) || \
        (rdp->other_modes.cycle_type == CYCLE_TYPE_1 && lod_frac_used_in_cc1))
        lodfracused = 1;

    if ((rdp->other_modes.cycle_type == CYCLE_TYPE_1 && rdp->combiner_rgbsub_a_r[1] == &rdp->noise) || \
        (rdp->other_modes.cycle_type == CYCLE_TYPE_2 && (rdp->combiner_rgbsub_a_r[0] == &rdp->noise || rdp->combiner_rgbsub_a_r[1] == &rdp->noise)) || \
        rdp->other_modes.alpha_dither_sel == 2)
        rdp->other_modes.f.getditherlevel = 0;
    else if (rdp->other_modes.f.rgb_alpha_dither != 0xf)
        rdp->other_modes.f.getditherlevel = 1;
    else
        rdp->other_modes.f.getditherlevel = 2;

    rdp->other_modes.f.dolod = rdp->other_modes.tex_lod_en || lodfracused;
}

struct rdp_state* rdp_create(uint32_t stride, uint32_t offset)
{
    struct rdp_state* state = calloc(1, sizeof(struct rdp_state));

    state->stride = stride;
    state->offset = offset;
    state->rseed = 3 + offset * 13;

    uint32_t tmp[2] = { 0 };
    rdp_set_other_modes(state, tmp);

    fb_init(state);
    combiner_init(state);
    tex_init(state);
    rasterizer_init(state);

    return state;
}

void rdp_destroy(struct rdp_state* rdp)
{
    if (rdp) {
        free(rdp);
    }
}

void rdp_invalid(struct rdp_state* rdp, const uint32_t* args)
{
}

void rdp_noop(struct rdp_state* rdp, const uint32_t* args)
{
}

void rdp_sync_load(struct rdp_state* rdp, const uint32_t* args)
{
}

void rdp_sync_pipe(struct rdp_state* rdp, const uint32_t* args)
{
}

void rdp_sync_tile(struct rdp_state* rdp, const uint32_t* args)
{
}

void rdp_sync_full(struct rdp_state* rdp, const uint32_t* args)
{
    // signal plugin to handle interrupts
    plugin_sync_dp();
}

void rdp_set_other_modes(struct rdp_state* rdp, const uint32_t* args)
{
    rdp->other_modes.cycle_type          = (args[0] >> 20) & 3;
    rdp->other_modes.persp_tex_en        = (args[0] >> 19) & 1;
    rdp->other_modes.detail_tex_en       = (args[0] >> 18) & 1;
    rdp->other_modes.sharpen_tex_en      = (args[0] >> 17) & 1;
    rdp->other_modes.tex_lod_en          = (args[0] >> 16) & 1;
    rdp->other_modes.en_tlut             = (args[0] >> 15) & 1;
    rdp->other_modes.tlut_type           = (args[0] >> 14) & 1;
    rdp->other_modes.sample_type         = (args[0] >> 13) & 1;
    rdp->other_modes.mid_texel           = (args[0] >> 12) & 1;
    rdp->other_modes.bi_lerp0            = (args[0] >> 11) & 1;
    rdp->other_modes.bi_lerp1            = (args[0] >> 10) & 1;
    rdp->other_modes.convert_one         = (args[0] >>  9) & 1;
    rdp->other_modes.key_en              = (args[0] >>  8) & 1;
    rdp->other_modes.rgb_dither_sel      = (args[0] >>  6) & 3;
    rdp->other_modes.alpha_dither_sel    = (args[0] >>  4) & 3;
    rdp->other_modes.blend_m1a_0         = (args[1] >> 30) & 3;
    rdp->other_modes.blend_m1a_1         = (args[1] >> 28) & 3;
    rdp->other_modes.blend_m1b_0         = (args[1] >> 26) & 3;
    rdp->other_modes.blend_m1b_1         = (args[1] >> 24) & 3;
    rdp->other_modes.blend_m2a_0         = (args[1] >> 22) & 3;
    rdp->other_modes.blend_m2a_1         = (args[1] >> 20) & 3;
    rdp->other_modes.blend_m2b_0         = (args[1] >> 18) & 3;
    rdp->other_modes.blend_m2b_1         = (args[1] >> 16) & 3;
    rdp->other_modes.force_blend         = (args[1] >> 14) & 1;
    rdp->other_modes.alpha_cvg_select    = (args[1] >> 13) & 1;
    rdp->other_modes.cvg_times_alpha     = (args[1] >> 12) & 1;
    rdp->other_modes.z_mode              = (args[1] >> 10) & 3;
    rdp->other_modes.cvg_dest            = (args[1] >>  8) & 3;
    rdp->other_modes.color_on_cvg        = (args[1] >>  7) & 1;
    rdp->other_modes.image_read_en       = (args[1] >>  6) & 1;
    rdp->other_modes.z_update_en         = (args[1] >>  5) & 1;
    rdp->other_modes.z_compare_en        = (args[1] >>  4) & 1;
    rdp->other_modes.antialias_en        = (args[1] >>  3) & 1;
    rdp->other_modes.z_source_sel        = (args[1] >>  2) & 1;
    rdp->other_modes.dither_alpha_en     = (args[1] >>  1) & 1;
    rdp->other_modes.alpha_compare_en    = (args[1] >>  0) & 1;

    set_blender_input(rdp, 0, 0, &rdp->blender1a_r[0], &rdp->blender1a_g[0], &rdp->blender1a_b[0], &rdp->blender1b_a[0],
                      rdp->other_modes.blend_m1a_0, rdp->other_modes.blend_m1b_0);
    set_blender_input(rdp, 0, 1, &rdp->blender2a_r[0], &rdp->blender2a_g[0], &rdp->blender2a_b[0], &rdp->blender2b_a[0],
                      rdp->other_modes.blend_m2a_0, rdp->other_modes.blend_m2b_0);
    set_blender_input(rdp, 1, 0, &rdp->blender1a_r[1], &rdp->blender1a_g[1], &rdp->blender1a_b[1], &rdp->blender1b_a[1],
                      rdp->other_modes.blend_m1a_1, rdp->other_modes.blend_m1b_1);
    set_blender_input(rdp, 1, 1, &rdp->blender2a_r[1], &rdp->blender2a_g[1], &rdp->blender2a_b[1], &rdp->blender2b_a[1],
                      rdp->other_modes.blend_m2a_1, rdp->other_modes.blend_m2b_1);

    rdp->other_modes.f.stalederivs = 1;
}

void rdp_cmd(struct rdp_state* rdp, const uint32_t* args)
{
    uint32_t cmd_id = CMD_ID(args);
    rdp_commands[cmd_id].handler(rdp, args);
}
