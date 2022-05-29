program test;
{Comment A}
var
    a, b, c: array [0..9, 0..9] of Integer;
    i, j, k: Integer;
begin
    for i := 0 to 2 do
        for j := 0 to 2 do begin
            a[i, j] := i + j;
            b[i, j] := (i + 1) * (j + 1);
        end;
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(a[i, j], ' ');
        writeln();
    end;
    writeln('*');
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(b[i, j], ' ');
        writeln();
    end;
    writeln('=');

    for i := 0 to 2 do
        for j := 0 to 2 do
            for k := 0 to 2 do 
                c[i, j] := c[i, j] + a[i, k] * b[k, j];
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(c[i, j], ' ');
        writeln();
    end;
end.