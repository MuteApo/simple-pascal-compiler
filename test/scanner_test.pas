program test;
{Comment A}
var
    i, j: Integer;
begin
    i := 1;
    while i <= 9 do begin
        j := 1;
        repeat
            writeln(i, '*', j, '=', i * j);
            j := j + 1;
        until j = 10;
        i := i + 1;
    end;
end.