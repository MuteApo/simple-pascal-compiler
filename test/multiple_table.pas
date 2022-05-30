program test;
{Comment A}
var
    i, j: Integer;
begin
    write(' i\j  ');
    for i := 1 to 9 do write(i, '  ');
    writeln();
    i := 1;
    while (i <= 9) do begin
        if i mod 2 = 0
            then write('[', i, ']: ')
            else write('(', i, '): ');
        j := 1;
        repeat
            if i * j < 10 then write(' ');
            write(i * j, ' ');
            j := j + 1;
        until j = 10;
        writeln();
        i := i + 1;
    end;
end.