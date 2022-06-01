program qsort;
var
    a: array [0..10000] of integer;
    n, i: integer;

procedure qsort(l, r: integer);
var
    i, j, p, t: integer;
begin
    i := l;
    j := r;
    p:= a[(l + r) div 2];
    repeat
        while a[i] < p do i := i + 1;
        while a[j] > p do j := j - 1;
        if i <= j then begin
            t := a[i];
            a[i] := a[j];
            a[j] := t;
            i := i + 1;
            j := j - 1;
        end;
    until i > j;
    if l < j then qsort(l, j);
    if i < r then qsort(i, r);
end;

begin
    read(n);
    for i := 1 to n do read(a[i]);
    qsort(1, n);
    for i := 1 to n do writeln(a[i]);
end.