program test;
{Comment A}
const
    z = 233;
    yy = 'y';
type
    week = (Mon, Tues, Wed, Thur, Fri);
    digit = 0..9;
    y = record
        i: integer;
        u: digit;
        v: ^week;
    end;
    strz = string[z];
var
    k: array [digit, 0..255] of y;
    a: integer;
    s: strz;

    function ff: week;
    type
        week2 = week;

        function fff(k: real): real;
        type
            week3 = week2;
        var
            p: week3;
        begin

        end;

    begin
        s := '114514';
        v := 1;  { identifier v is undefined }
    end;

    procedure gg(var x: integer; q: digit);
    var
        w: y;
    begin
        x := k[1, 'a'].u;  { expect integer, got digit(integer..integer), in deed this is correct, but not supported yet }
        x := k[2, 'b'].p;  { record type y does not have member r }
        w := k[3].c;       { illegal access to 2-dimension array k }
    end;

begin
    ff(1);     { ff can accept zero arguments }
    gg(a, 0);  { argument 2 expects digit(integer..integer), got integer, in deed this is correct, but no supported yet }
end.