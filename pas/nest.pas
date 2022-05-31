program env;
var 
    x: integer;
function a(m: integer; var n: integer):integer;
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
    {writeln(x);}
    {write(m, n);}
end;

begin
    x := 1;
    a(1, 2);
    writeln(x);
end.