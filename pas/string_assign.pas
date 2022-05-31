program test;
{Comment A}
var
    s1, s2: string;
    s3: array [0..10] of char;
    i: integer;
begin
    s1 := 'nihao';
    for i:= 0 to 4 do begin
        s3[i] := s1[i];
        write(s3[i], '#');
    end;
    for i:= 0 to 4 do s2[i] := s3[i];
    writeln(s2);
end.