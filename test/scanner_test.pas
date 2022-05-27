program test;
{Comment A}
var
    a, b: Integer;
begin
    a := 0;
    b := 1;
    repeat
        a := a + 1;
        b := b * a;
    until a = 3;
    write(b);
end.