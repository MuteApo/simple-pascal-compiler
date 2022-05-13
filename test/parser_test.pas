program test;
{Comment A}
const
    z = 114514;
type
    week = (Mon, Tues, Wed, Thur, Fri);
    digit = 0..9;
    y = set of (A, E, I, O, U);
var
    a: Integer;
    B: real;
    c: char;
    {d: string;}
    e: boolean;
    f: week;
    g: digit;
    h: set of week;
    i: set of digit;
    j: array [1..10] of integer;
    k: array [char] of week;
begin
    a := 3 mod 2 + 6 div 4;
    b := 4.5 / -1.2E-3; {Comment B}
    c := 'c';
    d := 'DeadBeef';
    e := (5 >= 4) and (3 <> 2);
    f := Fri;
    g := 8;
    a := j[4];
    k['a'] := Mon;
end