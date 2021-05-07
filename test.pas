
program a;
var
    b:integer;
function gcd(var a:integer):integer;
begin
    if a=0 then gcd:=1
    else gcd:=a*gcd(a-1);
end;
begin
    gcd(b);
end.
