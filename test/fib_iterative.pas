program test;
{Comment A}
var
    f: array [0..9] of Integer;
    i: Integer;
begin
    f[0] := 0;
    f[1] := 1;
    for i := 2 to 9 do begin
        f[i] := f[i - 1] + f[i - 2];
    end;
    for i := 0 to 9 do writeln('f', '(', i, ')', '=', f[i]);
end.