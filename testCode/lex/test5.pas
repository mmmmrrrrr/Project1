program testLex(input, output);
const 	
	ch = 'hwqywu';
var
	i, j, tmp, size: integer;
	list:array[0..1000] of integer;

begin
    for i := 1 to size-1 do
	for j := 1 to i do
	    if list[j] > list[j+1] then
	    begin
		    tmp := list[j];
		    list[j] := list[j+1];
		    list [j+1] := tmp;
	    end;

    for i :=1 to size do
	write(list[I]);
end.