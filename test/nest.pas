program env;

procedure a;
    var x: integer;
    procedure b;
    begin 
        x:=2;
        x:=x+1;
    end;
begin
    x:=3;
    x:=x+1;
end;

begin
    a();
end.