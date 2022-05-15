YACC = bison
YACC_PREFIX = parser
YACC_OUT = $(YACC_PREFIX).tab.c $(YACC_PREFIX).tab.h $(YACC_PREFIX).output

LEX = flex
LEX_PREFIX = scanner
LEX_OUT = $(LEX_PREFIX).yy.c

CC = g++
CFLAG = -O2
CC_SRC = main.cpp defs.cpp

EXE = ../../bin/pc

.PHONY: all clean

all: $(EXE)

$(YACC_OUT): $(YACC_PREFIX).y tree.hpp
	$(YACC) -o $(YACC_PREFIX).tab.c -d -v $<

$(LEX_OUT): $(LEX_PREFIX).l $(YACC_PREFIX).tab.h
	$(LEX) -o $(LEX_OUT) $< 

$(EXE): $(YACC_PREFIX).tab.c $(LEX_PREFIX).yy.c $(CC_SRC)
	$(CC) $^ ${CFLAG} -o $@

clean:
	$(shell rm -f $(YACC_OUT) $(LEX_OUT))