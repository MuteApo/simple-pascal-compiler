program test;
{Comment A}
var
    i, j: Integer;
begin
    for i := 1 to 9 do begin
        if i mod 2 = 0 then
            write('[', i, ']', ':', ' ')
        else write('(', i, ')', ':', ' ');
        j := 1;
        repeat
            write(i * j, ' ');
            j := j + 1;
        until j = 10;
        writeln(' ');
    end;
end.