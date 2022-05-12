bison -d parser.y
lex scanner.l
g++ parser.tab.c lex.yy.c tree.cpp -o pc
rm -rf parser.tab.c parser.tab.h lex.yy.c
./pc < ../../test/parser_test_simple.pas > ../../test/parser_test_simple.viz
dot ../../test/parser_test_simple.viz -Tpdf -o ../../test/parser_test_simple.pdf
