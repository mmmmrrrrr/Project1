1.	program test(input,output);  
2.	var a: array[0..5,6..10,11..15] of integer;  
3.	    b: integer;  
4.	begin  
5.	    a[0]:=b;  
6.	    b:=a[0, 6];  
7.	    a[0, 6, 11]:=b;  
8.	    b:=a[0, 6, 11, 16];  
9.	end.  