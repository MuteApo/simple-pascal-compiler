program advisor;
type
    planInfo = record
        id, taken: integer;
    end;
    courseInfo = record
        order, credit, score, groups: integer;
        size: array [1..7] of integer;
        pre: array [1..7, 1..7] of integer;
    end;
var
    s: string;
    i, j, k, l, x: integer;
    id, credit, groups, size, cnt: integer;
    totalScore, totalCredit, attemptCredit, completeCredit: integer;
    GPA: real;
    plan: array [1..100] of planInfo;
    course: array [0..999] of courseInfo;
begin
    read(s);
    while s[0] <> 0 do begin
        i := 1;
        x := 0;
        while s[i] <> '|' do begin
            x := x * 10 + s[i] - '0';
            i := i + 1;
        end;
        id := x;
        cnt := cnt + 1;
        plan[cnt].id := id;
        course[id].order := cnt;
        i := i + 1;
        credit := s[i] - '0';
        course[id].credit := credit;
        i := i + 1;
        j := 1;
        k := 1;
        if s[i + 1] <> '|'
        then repeat
            i := i + 1;
            if s[i] = 'c'
            then x := 0
            else course[id].pre[j, k] := x;
            if s[i] = ',' then k := k + 1;
            if s[i] = '|' then begin
                course[id].size[j] := k;
                course[id].groups := j;
            end;
            if s[i] = ';' then begin
                course[id].size[j] := k;
                j := j + 1;
                k := 1;
            end;
            if (s[i] >= '0') and (s[i] <= '9') then x := x * 10 + s[i] - '0';
        until s[i] = '|'
        else i := i + 1;
        i := i + 1;
        case s[i] of
            0: begin
                course[id].score := -1;
                totalCredit := totalCredit + credit;
            end;
            'F': begin
                course[id].score := 0;
                totalCredit := totalCredit + credit;
                attemptCredit := attemptCredit + credit;
            end;
            'A', 'B', 'C', 'D': begin
                course[id].score := 'E' - s[i];
                totalCredit := totalCredit + credit;
                attemptCredit := attemptCredit + credit;
                completeCredit := completeCredit + credit;
                totalScore := totalScore + credit * course[id].score;
            end;
        end;
        read(s);
    end;
    
    if attemptCredit = 0
    then GPA := 0
    else GPA := totalScore / attemptCredit;
    writeln('GPA: ', GPA);
    writeln('Hours Attempted: ', attemptCredit);
    writeln('Hours Completed: ', completeCredit);
    writeln('Credits Remaining: ', totalCredit - completeCredit);
    writeln();
    writeln('Possible Courses to Take Next');
    if totalCredit = completeCredit
    then writeln('  None - Congratulations!')
    else begin
        for i := 1 to cnt do begin
            x := plan[i].id;
            if course[x].score > 0 then
                for j := 1 to cnt do begin
                    id := plan[j].id;
                    groups := course[id].groups;
                    for k := 1 to groups do begin
                        size := course[id].size[k];
                        for l := 1 to size  do
                            if course[id].pre[k, l] = x then course[id].pre[k, l] := -1;
                    end;
                end;
        end;

        {for i := 1 to cnt do begin
            id := plan[i].id;
            writeln('course id: ', id);
            writeln('course credit: ', course[id].credit);
            writeln('course requisition sets: ', course[id].groups);
            for j := 1 to course[id].groups do begin
                write('   set ', j, ': ');
                for k := 1 to course[id].pre[j, 0] do 
                    write(course[id].pre[j, k], ' ');
                writeln();
            end;
            writeln('course score: ', course[id].score);
            writeln();
        end;}

        for i := 1 to cnt do begin
            id := plan[i].id;
            if course[id].score <= 0 then begin
                groups := course[id].groups;
                if groups = 0
                then plan[i].taken := 1
                else for j := 1 to groups do begin
                    x := 1;
                    size := course[id].size[j];
                    for k := 1 to size do
                        if course[id].pre[j, k] <> -1 then x := 0;
                    if x = 1 then plan[i].taken := 1;
                end;
            end;
        end;

        for i := 1 to cnt do
            if plan[i].taken = 1 then writeln('  c', plan[i].id);
    end;
end.