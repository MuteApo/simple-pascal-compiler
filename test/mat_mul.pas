program test;
{Comment A}
type
    int = record
        dumb: array [0..9] of integer;
        x: integer;
    end;
var
    a, b, c: array [0..9, 0..9] of int;
    i, j, k: Integer;
begin
    for i := 0 to 2 do
        for j := 0 to 2 do begin
            a[i, j].x := i + j;
            b[i, j].x := -i - j;
        end;
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(a[i, j].x, ' ');
        writeln();
    end;
    writeln('*');
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(b[i, j].x, ' ');
        writeln();
    end;
    writeln('=');

    for i := 0 to 2 do begin
        for j := 0 to 2 do begin
            for k := 0 to 2 do
                c[i, j].x := c[i, j].x + a[i, k].x * b[k, j].x;
            write(c[i, j].x, ' ');
        end;
        writeln();
    end;
end.