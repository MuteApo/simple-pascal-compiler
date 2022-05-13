bison -d parser.y
lex scanner.l
g++ parser.tab.c lex.yy.c tree.cpp -o pc
./pc < ../../test/parser_test.pas > parser_test.viz
dot -Tpng parser_test.viz -o ../../visual/parser_test.png
rm -rf parser.tab.c parser.tab.h lex.yy.c parser_test.viz
