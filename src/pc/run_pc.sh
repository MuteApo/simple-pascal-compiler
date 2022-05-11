bison -d yacc.y
lex lex.l
g++ yacc.tab.c lex.yy.c -o pc
./pc < test.pas > out.txt
