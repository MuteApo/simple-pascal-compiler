program qsort;
var
    a: array [0..10000] of integer;
    n, i: integer;

procedure qsort(l, r: integer);
var
    i, j, mid: integer;
begin
    i := l;
    j := r;
    mid:= a[(l + r) div 2];
    repeat
        while a[i] < mid do i := i + 1;
        while a[j] > mid do j := j - 1;
        if i <= j then begin
            a[0] := a[i];
            a[i] := a[j];
            a[j] := a[0];
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