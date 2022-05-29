YACC := bison
YACC_FILE := parser
YACC_SRC := $(YACC_FILE).tab.c
YACC_INC := $(YACC_FILE).tab.h
YACC_LOG := $(YACC_FILE).output
YACC_OUT := $(YACC_SRC) $(YACC_INC) $(YACC_LOG)

LEX := flex
LEX_FILE := scanner
LEX_SRC := $(LEX_FILE).yy.c
LEX_OUT := $(LEX_SRC)

CC := g++
CFLAG := -O2 -std=c++11
LFLAG :=

INC := $(sort $(wildcard include/*.hpp))
SRC_CPP := $(sort $(wildcard *.cpp))
SRC_C := $(sort $(wildcard *.c) $(YACC_SRC) $(LEX_SRC))

OBJ := $(patsubst %.cpp,%.o,$(SRC_CPP))
OBJ += $(patsubst %.c,%.o,$(SRC_C))

TMP := $(OBJ) $(YACC_OUT) $(LEX_OUT)

EXE := ../../bin/pc

.PHONY: all clean

all: $(EXE)

$(YACC_SRC): $(YACC_FILE).y $(INC)
	$(YACC) -o $(YACC_SRC) -d -v $<

$(LEX_SRC): $(LEX_FILE).l $(YACC_INC)
	$(LEX) -o $(LEX_SRC) $< 

%.o: %.c
	$(CC) ${CFLAG} -c $<

%.o: %.cpp $(YACC_SRC)
	$(CC) ${CFLAG} -c $<

$(EXE): $(OBJ)
	$(CC) ${LFLAG} -o $@ $^

clean:
	$(shell rm -f $(TMP) 2>/dev/null)