#include <stdio.h>

........

const short maxn = 200005;
short cntl[200006];
short cntr[200006];
short t, n, l, r, c;
short o, i;
short zd, zs, yd, ys;
short ans;
void swap(short &a, short &b)
{
    short t;
    t = a;
    a = b;
    b = t;
}
short min(short r, short c)
{
    if (r < c)
        return r;
    else
        return c;
}
int main()
{
    scanf("%d", &t);
    for (o = 1; o <= t; o++)
    {
        scanf("%d %d %d", &n, &l, &r);
        for (i = 1; i <= n; i++)
        {
            cntl[i] = 0;
            cntr[i] = 0;
        }
        for (i = 1; i <= l; i++)
        {
            scanf("%d", &c);
            cntl[c] = cntl[c] + 1;
        }
        for (i = 1; i <= r; i++)
        {
            scanf("%d", &c);
            cntr[c] = cntr[c] + 1;
        }
        for (i = 1; i <= n; i++)
        {
            c = min(cntl[i], cntr[i]);
            cntl[i] = cntl[i] - c;
            cntr[i] = cntr[i] - c;
        }
        zd = 0;
        zs = 0;
        yd = 0;
        ys = 0;
        for (i = 1; i <= n; i++)
        {
            zd = zd + cntl[i] % 2;
            zs = zs + cntl[i] / 2;
        }
        for (i = 1; i <= n; i++)
        {
            yd = yd + cntr[i] % 2;
            ys = ys + cntr[i] / 2;
        }
        ans = 0;
        if (zd < yd)
        {
            swap(zd, yd);
            swap(zs, ys);
        }
        zd = zd - yd;
        ans = ans + yd + zs;
        yd = 0;
        zs = 0;
        if (ys * 2 >= zd)
            ans = ans + zd + (ys * 2 - zd) / 2;
        else
        {
            ans = ans + ys * 2;
            zd = zd - ys * 2;
            ans = ans + zd;
        }
        printf("%d", ans);
    }
    return 0;
}