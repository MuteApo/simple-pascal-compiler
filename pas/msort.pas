program msort;
var
    a, b: array [0..10000] of integer;
    n, i: integer;

procedure msort(l, r: integer);
var
    m, i: integer;

    { procedure sort(l, r: integer); m is accessed as non-global-non-local var in msort(), rather param to sort() }
    procedure sort(l, m, r: integer); 
    var
        i, j, k: integer;
    begin
        i := l;
        j := m + 1;
        k := l;
        while (i <= m) and (j <= r) do
            if a[i] <= a[j] then begin
                b[k] := a[i];
                k := k + 1;
                i := i + 1;
            end else begin
                b[k] := a[j];
                k := k + 1;
                j := j + 1;
            end;
        while i <= m do begin
            b[k] := a[i];
            k := k + 1;
            i := i + 1;
        end;
        while j <= r do begin
            b[k] := a[j];
            k := k + 1;
            j := j + 1;
        end;
    end;

begin
    m := (l + r) div 2;
    if l < r then begin
        msort(l, m);
        msort(m + 1, r);
    end;
    { sort(l, r); }
    sort(l, m, r);
    for i := l to r do a[i] := b[i];
end;

begin
    read(n);
    for i := 1 to n do read(a[i]);
    msort(1, n);
    for i := 1 to n do writeln(a[i]);
end.