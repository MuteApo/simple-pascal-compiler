program test;
{Comment A}
var
    i, j: Integer;
begin
    i := 1;
    while i <= 9 do begin
        if i mod 2 = 0 then
            write('[', i, ']', ':', ' ')
        else write('(', i, ')', ':', ' ');
        j := 1;
        repeat
            write(i * j, ' ');
            j := j + 1;
        until j = 10;
        i := i + 1;
        writeln(' ');
    end;
end.