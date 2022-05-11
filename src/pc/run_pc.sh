bison -d yacc.y
lex lex.l
g++ yacc.tab.c lex.yy.c -o pc
rm -rf yacc.tab.c yacc.tab.h lex.yy.c
./pc < ../../test/test.pas
