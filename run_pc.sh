cd src/pc
bison -d parser.y
lex scanner.l
g++ parser.tab.c lex.yy.c defs.cpp -o ../../bin/pc
rm -rf parser.tab.c parser.tab.h lex.yy.c
cd ../..
bin/pc < test/parser_test.pas > bin/parser_test.viz
dot -Tpng bin/parser_test.viz -o bin/parser_test.png
