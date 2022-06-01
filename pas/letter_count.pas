program test;
{Comment A}
var
    s: string;
    a: array ['a'..'z'] of integer;
    i: integer;
    j: char;
begin
    s:= 'please enter a word: ';
    write(s);
    read(s);
    i := 0;
    while s[i] <> 0 do begin
        a[s[i]] := a[s[i]] + 1;
        i := i + 1;
    end;
    for j := 'a' to 'z' do
        if a[j] <> 0 then writeln(j, ': ', a[j]);
end.