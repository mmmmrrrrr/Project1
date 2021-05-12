#include <stdio.h>

const short maxn=200005;
short cntl[200006];
short cntr[200006];
short t,n,l,r,c;
short o,i;
short zd,zs,yd,ys;
short ans;
void swap(short &a,short &b)
{
short t;
t=a;
a=b;
b=t;
}
short min(short r,short c)
{
short min_returnVal;
if(r<c) min_returnVal=r;
else min_returnVal=c;
return min_returnVal;
}
int main()
{
scanf("%hd",&t);
for(o=1;o<=t;o++) {
scanf("%hd %hd %hd",&n,&l,&r);
for(i=1;i<=n;i++) {
cntl[i-0]=0;
cntr[i-0]=0;
}
for(i=1;i<=l;i++) {
scanf("%hd",&c);
cntl[c-0]=cntl[c-0]+1;
}
for(i=1;i<=r;i++) {
scanf("%hd",&c);
cntr[c-0]=cntr[c-0]+1;
}
for(i=1;i<=n;i++) {
c=min(cntl[i-0],cntr[i-0]);
cntl[i-0]=cntl[i-0]-c;
cntr[i-0]=cntr[i-0]-c;
}
zd=0;
zs=0;
yd=0;
ys=0;
for(i=1;i<=n;i++) {
zd=zd+cntl[i-0]%2;
zs=zs+cntl[i-0]/2;
}
for(i=1;i<=n;i++) {
yd=yd+cntr[i-0]%2;
ys=ys+cntr[i-0]/2;
}
ans=0;
if(zd<yd) {
swap(zd,yd);
swap(zs,ys);
}
zd=zd-yd;
ans=ans+yd+zs;
yd=0;
zs=0;
if(ys*2>=zd) ans=ans+zd+(ys*2-zd)/2;
else {
ans=ans+ys*2;
zd=zd-ys*2;
ans=ans+zd;
}
printf("%hd",ans);
}
return 0;
}
