YACC := bison
YACC_FILE := parser
YACC_SRC := $(YACC_FILE).tab.cpp
YACC_INC := $(YACC_FILE).tab.hpp
YACC_LOG := $(YACC_FILE).output
YACC_OUT := $(YACC_SRC) $(YACC_INC) $(YACC_LOG)

LEX := flex
LEX_FILE := scanner
LEX_SRC := $(LEX_FILE).yy.cpp
LEX_OUT := $(LEX_SRC)

SRC := $(sort $(wildcard *.cpp) $(YACC_SRC) $(LEX_SRC))
INC := $(sort $(wildcard include/*.hpp))
OBJ := $(patsubst %.cpp,%.o,$(SRC))

TMP := $(OBJ) $(YACC_OUT) $(LEX_OUT)

EXE := ../../bin/pc

.PHONY: all clean

all: $(EXE)

$(YACC_SRC): $(YACC_FILE).y $(INC)
	$(YACC) -o $(YACC_SRC) -d -v $<

$(LEX_SRC): $(LEX_FILE).l $(YACC_SRC)
	$(LEX) -o $(LEX_SRC) $< 

%.o: %.cpp $(YACC_SRC)
	$(CC) $(CFLAG) -c $<

$(EXE): $(OBJ)
	$(CC) $(LFLAG) -o $@ $^

clean:
	$(shell rm -f $(TMP) 2>/dev/null)