program test;
{Comment A}
const
    z = 233;
type
    week = (Mon, Tues, Wed, Thur, Fri);
    digit = 0..9;
    y = set of (A, E, I, O, U);
var
    d: string;
    d1: string[233];
    d2: string[z];
    k: array [1..10, char] of record
        i: set of digit;
        u: week;
        v: ^integer;
    end;
function ff: week;
begin

end;
procedure gg(var x: integer; q: digit);
begin
    x := k[1, 'a'].v;
end;
begin
    
end.