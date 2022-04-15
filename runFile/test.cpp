#include <stdio.h>

short x,y;
short gcd(short a,short b)
{
short gcd_returnVal;
if(b==0) gcd_returnVal=a;
else gcd_returnVal=gcd(b,a%b);
return gcd_returnVal;
}
int main()
{
scanf("%hd %hd",&x,&y);
printf("%hd",gcd(x,y));
return 0;
}
