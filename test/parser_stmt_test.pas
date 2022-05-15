program test;
{Comment A}
const
    z = 233;
type
    week = (Mon, Tues, Wed, Thur, Fri);
    digit = 0..9;
    y = set of (A, E, I, O, U);
function ff:week;
begin
    ff := Fri;
end;
procedure gg(var x: integer; y: digit);
begin
    if y > 0 then x := x + 1;
end;
begin
    a := 3 mod 2 + 6 div 4;
    b := 4.5 / -1.2E-3; {Comment B}
    d := 'DeadBeef';
    read(a);
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
    write(ff());
    gg(j[1], g);
end.