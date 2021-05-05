program a;
const 
    maxN=200005;
var
    cntL,cntR:array [0..200005] of integer;
    T,n,l,r,c:integer;
    o,i:integer;
    zd,zs,yd,ys:integer;
    ans:integer;
procedure swap(var a,b:integer);
var
    t:integer;
begin
    t:=a;
    a:=b;
    b:=t;
end;
function min(r,c:integer):integer;
begin
    if r<c then
        min:=r
    else min:=c;
end;
 begin
    read(T);
    for o:=1 to T do begin
        read(n,l,r);
        for i:=1 to n do begin
            cntL[i]:=0;
            cntR[i]:=0;
        end;
        for i:=1 to l do begin
            read(c);
            cntL[c]:=cntL[c]+1;
        end;
        for i:=1 to r do begin
            read(c);
            cntR[c]:=cntR[c]+1;
        end;
        for i:=1 to n do begin
            c:=min(cntL[i],cntR[i]);
            cntL[i]:=cntL[i]-c;
            cntR[i]:=cntR[i]-c;
        end;
        zd:=0;
        zs:=0;
        yd:=0;
        ys:=0;
        for i:=1 to n do begin
            zd:=zd+cntL[i] mod 2;
            zs:=zs+cntL[i] div 2;
        end;
        for i:=1 to n do begin
            yd:=yd+cntR[i] mod 2;
            ys:=ys+cntR[i] div 2;
        end;

        ans:=0;
        if zd<yd then begin
            swap(zd,yd);
            swap(zs,ys);
        end;
        zd:=zd-yd;
        ans:=ans+yd+zs;
        yd:=0;
        zs:=0;
        if ys*2>=zd then begin
            ans:=ans+zd+(ys*2-zd)div 2;
        end
        else begin
            ans:=ans+ys*2;
            zd:=zd-ys*2;
            ans:=ans+zd;
        end;
        write(ans);
    end;

 end.