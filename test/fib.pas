program recursiveFibonacci;
var
    i: integer;
function fibonacci(n: integer): integer;

begin
    if n=1 then fibonacci := 0
    else if n=2 then fibonacci := 1
    else fibonacci := fibonacci(n-1) + fibonacci(n-2);
end; 

begin
    for i:= 1 to 10 do fibonacci (i);
end.