program fibr;
var
    i: integer;

function fib(n: integer): integer;
begin
    if n = 1 then fib := 0
    else if n = 2 then fib := 1
    else fib := fib(n - 1) + fib(n - 2);
end; 

begin
    for i := 1 to 10 do writeln('fib(', i, ') = ', fib(i));
end.