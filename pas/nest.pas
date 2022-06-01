program env;
var 
    x: integer;
function a(m: integer; n: integer):integer;
    var
        x: integer;
    procedure b;
    begin 
        {x := 3;
        writeln(x);}
        writeln(3);
    end;
begin
    x:=2;
    writeln(x);
    b;
    a := m + n;
end;

begin
    x := a(4, 5);
    writeln(x);
end.