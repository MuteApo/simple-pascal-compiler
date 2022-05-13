LEX = flex
YACC = bison
CC = g++
CFLAG = -O2

LEX_SRC = scanner.l
YACC_SRC = parser.y
CC_SRC = main.cpp defs.cpp lex.yy.c parser.tab.c
TMP = lex.yy.c parser.tab.c parser.tab.h
EXE = ../../bin/pc

.PHONY: before all clean

before:$(YACC_SRC) $(LEX_SRC)
	$(YACC) -d $(YACC_SRC)
	$(LEX) $(LEX_SRC)

all: before $(CC_SRC)
	$(CC) $(CC_SRC) ${CFLAG} -o $(EXE)

clean:
	$(shell rm -f $(TMP))