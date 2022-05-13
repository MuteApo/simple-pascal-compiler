LEX = flex
YACC = bison
CC = g++
CFLAG = -O2

LEX_SRC = scanner.l
YACC_SRC = parser.y
CC_SRC = main.cpp lex.yy.c parser.tab.c defs.cpp
TMP = lex.yy.c parser.tab.c parser.tab.h
EXE = ../../bin/pc

all:$(OBJ)
	$(YACC) -d $(YACC_SRC)
	$(LEX) $(LEX_SRC)
	$(CC) $(CC_SRC) ${CFLAG} -o $(EXE)

clean:
	$(shell rm -f $(TMP))