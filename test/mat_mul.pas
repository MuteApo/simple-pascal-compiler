program test;
{Comment A}
var
    a, b, c: array [0..30, 0..30] of integer;
    am, an, bm, bn: integer;
    i, j, k, x, l: Integer;
begin
    read(am, an);
    for i := 0 to am - 1 do
        for j := 0 to an - 1 do 
            read(a[i, j]);
    read(bm, bn);
    for i := 0 to bm - 1 do
        for j := 0 to bn - 1 do 
            read(b[i, j]);

    if an <> bm
    then writeln('Incompatible Dimensions')
    else begin
        for i := 0 to am - 1 do begin
            for j := 0 to bn - 1 do begin
                for k := 0 to an - 1 do
                    c[i, j] := c[i, j] + a[i, k] * b[k, j];

                x := c[i, j];
                l := 10;
                if x < 0 then l := l - 1;
                while x <> 0 do begin
                    l := l - 1;
                    x := x div 10;
                end;
                for x := 1 to l do write(' ');
                
                write(c[i, j]);
            end;
            writeln();
        end;
    end;


end.