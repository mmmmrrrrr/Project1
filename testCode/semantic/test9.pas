program test(input,output);  
var a: array[0..5,6..10,11..15] of integer;  
    b: integer;  
begin  
    a[0]:=b;  
    b:=a[0, 6];  
    a[0, 6, 11]:=b;  
    b:=a[0, 6, 11, 16];  
end.  