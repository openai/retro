static STRICTINLINE void divot_filter(struct rgba* final, struct rgba center, struct rgba left, struct rgba right)
{
    *final = center;

    if ((center.a & left.a & right.a) == 7)
    {
        return;
    }

    if ((left.r >= center.r && right.r >= left.r) || (left.r >= right.r && center.r >= left.r))
        final->r = left.r;
    else if ((right.r >= center.r && left.r >= right.r) || (right.r >= left.r && center.r >= right.r))
        final->r = right.r;

    if ((left.g >= center.g && right.g >= left.g) || (left.g >= right.g && center.g >= left.g))
        final->g = left.g;
    else if ((right.g >= center.g && left.g >= right.g) || (right.g >= left.g && center.g >= right.g))
        final->g = right.g;

    if ((left.b >= center.b && right.b >= left.b) || (left.b >= right.b && center.b >= left.b))
        final->b = left.b;
    else if ((right.b >= center.b && left.b >= right.b) || (right.b >= left.b && center.b >= right.b))
        final->b = right.b;
}
