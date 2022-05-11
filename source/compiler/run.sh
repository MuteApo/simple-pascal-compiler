bison -d yacc.y
lex lex.l
g++ yacc.tab.c lex.yy.c
./a.out < test.pas
