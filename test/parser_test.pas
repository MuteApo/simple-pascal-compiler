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
    l: record
        u: week;
        v: integer;
    end;
    h: set of week;
    i: set of digit;
    j: array [1..10] of integer;
    k: array [char] of week;
function ff:week;
begin
    ff := k['z'];
end;
procedure gg(var x: integer; y: digit);
begin
    if y > 0 then x := x + 1;
end;
begin
    c := 'c';
    a := 3 mod 2 + 6 div 4;
    b := 4.5 / -1.2E-3; {Comment B}
    d := 'DeadBeef';
    a := 0;
    repeat
        for i := 1 to 9 do begin
            case (a) of
                0, 1: j := i;
                2: j := i - 1;
                3: j := i + 1;
            end;
            while j > 0 do begin
                if (5 >= 4) and (3 <> 2)
                    then if (i <= 5) 
                        then e := true
                        else e := false;
                j := j - 1;
            end;
        end;
        a := a + 1;
    until a > 3;
    f := ff();
    gg(j[1], g);
end