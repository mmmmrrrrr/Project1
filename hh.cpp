#include <stdio.h>

short n, i;
short list[1001];
char c;
void qsort(short low, short high)
{
    short l, h, m;
    short i, j;
    short temp;
    short flag;
    flag = 0;
    l = low;
    h = high;
    m = list[(l + h) / 2 - 0];
    for (i = 1; i <= 1000; i++)
    {
        if (flag == 0)
        {
            for (j = 1; j <= 1000; j++)
                if (list[l - 0] < m)
                    l = l + 1;
            for (j = 1; j <= 1000; j++)
                if (list[h - 0] > m)
                    h = h - 1;
            if (l <= h)
            {
                temp = list[l - 0];
                list[l - 0] = list[h - 0];
                list[h - 0] = temp;
                l = l + 1;
                h = h - 1;
            }
            if ((l > h))
                flag = 1;
        }
    }
    if (l < high)
        qsort(l, high);
    if (h > low)
        qsort(low, h);
}
int main()
{
    scanf("%hd", &n);
    for (i = 0; i <= n - 1; i++)
        scanf("%hd", &list[i - 0]);
    qsort(0, n - 1);
    for (i = 0; i <= n - 1; i++)
        printf("%hd", list[i - 0]);
    return 0;
}