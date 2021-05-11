program example(input,output);
    var x,y:integer;
    function gcd(a,b:integer):integer;
    var d:integer;
        begin 
            if b=0 then gcd:=a
            else gcd:=b;
            gcd:=1;
            d:=gcd;
        end;
    begin
        read(x, y);
        write(gcd(x, y))
    end.