program matmul;
{Comment A}
var
    a, b: array [1..30, 1..30] of integer;
    am, an, bm, bn: integer;
    i, j, k, x: Integer;

procedure print(x: integer);
var
    y, l: integer;
begin
    y := x;
    if x < 0
    then l := 9
    else l := 10;

    while x <> 0 do begin
        x := x div 10;
        l := l - 1;
    end;

    while l <> 0 do begin
        write(' ');
        l := l - 1;
    end;

    write(y);
end;

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
                x := 0;
                for k := 1 to an do
                    x := x + a[i, k] * b[k, j];

                print(x);
            end;
            writeln();
        end;
    end;
end.