program env;
var 
    x: integer;
function a(m: integer; n: integer):integer;
    var
        x: integer;
    procedure b;
        procedure c;
        begin
            writeln(x);
            x := 4;
        end;
    begin 
        x := 3;
        c;
    end;
begin
    x := 2; 
    b;
    writeln(x);
    a := m + n;
end;

begin
    x := a(4, 5);
    writeln(x);
end.