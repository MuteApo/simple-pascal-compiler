program test;
{Comment A}
var
    a: Integer;
    b, G: real;
    c: char;
    d, f: string;
    e: boolean;
    h: (Mon, Tues, Wed, Thur, Fri);
    i: 1..10;
    j: -10..-1;
    k: set of h;
    l: set of i;
    m: array [1..10] of integer;
    n: array [char] of h;
begin
    a := 1 + 2 * 3;
    b := 4.5 / 3; {Comment B}
    c := 'c';
    d := 'DeadBeef';
    e := (5 >= 4) and (3 <> 2);
    f := '''';
    G := -1.2E-3;
    m[5] := 9;
    n['a'] := Mon;
    a := m[4];
end