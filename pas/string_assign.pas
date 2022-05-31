program test;
{Comment A}
var
    s1, s2: string;
begin
    s1 := 'nihao';
    s2 := 'hihi';
    writeln(s1, '#', s2);
    s2 := s1;
    writeln(s1, '#', s2);
    s2 := 'hello';
    writeln(s1, '#', s2);
    s1 := s2;
    writeln(s1, '#', s2);
end.