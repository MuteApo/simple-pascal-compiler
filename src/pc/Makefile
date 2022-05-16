YACC = bison
YACC_PREFIX = parser
YACC_OUT = $(YACC_PREFIX).tab.c $(YACC_PREFIX).tab.h $(YACC_PREFIX).output

LEX = flex
LEX_PREFIX = scanner
LEX_OUT = $(LEX_PREFIX).yy.c

CC = g++
CFLAG = -O2
LFLAG = -static-libgcc

SRC_C = $(YACC_PREFIX).tab.c $(LEX_PREFIX).yy.c
SRC_CPP = $(sort $(wildcard *.cpp))
INC = $(sort $(wildcard include/*.hpp))

OBJ_C = $(patsubst %.c,%.o,$(SRC_C))
OBJ_CPP = $(patsubst %.cpp,%.o,$(SRC_CPP))

TMP = $(OBJ_C) $(OBJ_CPP) $(YACC_OUT) $(LEX_OUT)

EXE = ../../bin/pc

.PHONY: all clean

all: $(EXE)

$(YACC_OUT): $(YACC_PREFIX).y $(INC)
	$(YACC) -o $(YACC_PREFIX).tab.c -d -v $<

$(LEX_OUT): $(LEX_PREFIX).l $(YACC_PREFIX).tab.h
	$(LEX) -o $(LEX_OUT) $< 

$(OBJ_C) $(OBJ_CPP): $(SRC_C) $(SRC_CPP)
	$(CC) ${CFLAG} -c $^

$(EXE): $(OBJ_C) $(OBJ_CPP)
	$(CC) ${LFLAG} -o $@ $^

clean:
	$(shell rm -f $(TMP) 2>/dev/null)