program test;
{Comment A}
var
    a, b, c: array [1..30, 1..30] of integer;
    am, an, bm, bn: integer;
    i, j, k, x, l: Integer;
begin
    read(am, an);
    for i := 1 to am do
        for j := 1 to an do 
            read(a[i, j]);
    read(bm, bn);
    for i := 1 to bm do
        for j := 1 to bn do 
            read(b[i, j]);

    if an <> bm
    then writeln('Incompatible Dimensions')
    else begin
        for i := 1 to am do begin
            for j := 1 to bn do begin
                for k := 1 to an do
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