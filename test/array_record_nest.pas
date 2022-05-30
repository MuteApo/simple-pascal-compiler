program test;
{Comment A}
type
    arr = record
        a: integer;
    end;
    intarr = record
        dumb: integer;
        x: array [0..9, 0..9] of integer;
    end;
var
    a, b, c: intarr;
    i, j, k: Integer;
begin
    for i := 0 to 2 do
        for j := 0 to 2 do begin
            a.x[i, j] := i + j;
            b.x[i, j] := -i - j;
        end;
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(a.x[i, j], ' ');
        writeln();
    end;
    writeln('*');
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(b.x[i, j], ' ');
        writeln();
    end;
    writeln('=');

    for i := 0 to 2 do begin
        for j := 0 to 2 do begin
            for k := 0 to 2 do
                c.x[i, j] := c.x[i, j] + a.x[i, k] * b.x[k, j];
            write(c.x[i, j], ' ');
        end;
        writeln();
    end;
end.