bison -d parser.y
lex scanner.l
g++ parser.tab.c lex.yy.c -o pc
rm -rf parser.tab.c parser.tab.h lex.yy.c
./pc < ../../test/parser_test_simple.pas
