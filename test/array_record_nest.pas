program test;
{Comment A}
type
    arr = record
        dumb: char;
        a: integer;
    end;
    int = record
        dumb: arr;
        x: array [0..2] of arr;
    end;
var
    a, b, c: intarr;
    i, j, k: Integer;
begin
    for i := 0 to 2 do
        for j := 0 to 2 do begin
            a[i, j].x[0].a := i + j;
            b[i, j].x[0].a := -i - j;
        end;
    
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(a[i, j].x[0].a, ' ');
        writeln();
    end;
    writeln('*');
    for i := 0 to 2 do begin
        for j := 0 to 2 do 
            write(b[i, j].x[0].a, ' ');
        writeln();
    end;
    writeln('=');

    for i := 0 to 2 do
        for j := 0 to 2 do
            for k := 0 to 2 do
                c[i, j].x[0].a := c[i, j].x[0].a + a[i, k].x[0].a * b[k, j].x[0].a;

    for i := 0 to 2 do begin
        for j := 0 to 2 do
            write(c[i, j].x[0].a, ' ');
        writeln();
    end;
end.