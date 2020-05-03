static STRICTINLINE void vi_vl_lerp(struct rgba* up, struct rgba down, uint32_t frac)
{
    uint32_t r0, g0, b0;
    if (!frac)
        return;

    r0 = up->r;
    g0 = up->g;
    b0 = up->b;

    up->r = ((((down.r - r0) * frac + 16) >> 5) + r0) & 0xff;
    up->g = ((((down.g - g0) * frac + 16) >> 5) + g0) & 0xff;
    up->b = ((((down.b - b0) * frac + 16) >> 5) + b0) & 0xff;
}
