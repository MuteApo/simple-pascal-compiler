program env;
var x: integer;
procedure a;
    var
        x: array [0..1] of integer;
        y: integer;
    procedure b;
    begin 
        {x := 2;
        writeln(x);}
    end;
begin
    x[0] := 3;
    x[1] := x[0] * 2;
    y := x[1] + 1;
    b;
    writeln(y);
end;

begin
    x := 1 + 6 div 3;
    a;
    writeln(x);
end.