program qsort(input,output);
var
	n,i:integer;
	list:array[0..1000] of integer;
	c:char;
procedure qsort(low, high:integer);
var
	l,h,m:integer;
	i,j:integer;
	temp:integer;
	flag:integer;
begin
	flag:=0;
	l:=low; h:=high;
	m:=list[(l+h) div 2];
	for i:=1 to 1000 do
	begin
	if flag=0 then begin
		for j:=1 to 1000 do
			if list[l]<m then l:=l+1;
		for j:=1 to 1000 do
			if list[h]>m then h:=h-1;
		if l<=h then
		begin
			temp:=list[l]; list[l]:=list[h]; list[h]:=temp;
			l:=l+1; h:=h-1;
		end;
		if (l>h) then flag:=1;
	end;
	end;
	if l<high then qsort(l,high);
	if h>low then qsort(low,h);
end;

begin
	read(n);
	for i:=0 to n-1 do
		read(list[i]);
	qsort(0,n-1);
        for i:=0 to n-1 do
        begin
            write(list[i]);
        end
end.
