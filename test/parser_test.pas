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
    f := Fri;
    g := 8;
    a := j[4];
    k['a'] := Mon;
end