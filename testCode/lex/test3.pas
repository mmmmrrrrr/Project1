program testLex(input, output);
var
	i@djdhs, j, tmp, size: integer;
	list:array[0..1000] of integer;	
begin
    for i@djdhs := 1 to size-1 do
	for j := 1 to i@djdhs do
	    if list[j] > list[j+1] then
	    begin
		    tmp := list[j];
		    list[j] := list[j+1];
		    list [j+1] := tmp;
	    end;

    for i@djdhs :=1 to size do
	write(list[i@djdhs])
end.