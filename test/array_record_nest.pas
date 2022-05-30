program test;
{Comment A}
type
    arr = record
        a: integer;
    end;
    int = record
        dumb: integer;
        x: array [0..0] of integer;
    end;
var
    a, b, c: array [0..9, 0..9] of int;
    i, j, k: Integer;
begin
    for i := 0 to 2 do
        for j := 0 to 2 do begin
            a[i, j].x[0] := i + j;
            b[i, j].x[0] := -i - j;
        end;
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(a[i, j].x[0], ' ');
        writeln();
    end;
    writeln('*');
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(b[i, j].x[0], ' ');
        writeln();
    end;
    writeln('=');

    for i := 0 to 2 do begin
        for j := 0 to 2 do begin
            for k := 0 to 2 do
                c[i, j].x[0] := c[i, j].x[0] + a[i, k].x[0] * b[k, j].x[0];
            write(c[i, j].x[0], ' ');
        end;
        writeln();
    end;
end.