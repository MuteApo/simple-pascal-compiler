program test;
{Comment A}
const
    z = 114514;
    y = set of (A, E, I, O, U);
var
    a: Integer;
    B: real;
    c: char;
    d: string;
    e: boolean;
    f: (Mon, Tues, Wed, Thur, Fri);
    g: 1..10;
    h: -10..-1;
    i: set of f;
    j: set of h;
    k: array [1..10] of integer;
    l: array [char] of f;
begin
    a := 3 mod 2 + 6 div 4;
    b := 4.5 / -1.2E-3; {Comment B}
    c := 'c';
    d := 'DeadBeef';
    e := (5 >= 4) and (3 <> 2);
    a := k[4];
    l['a'] := Mon;
end