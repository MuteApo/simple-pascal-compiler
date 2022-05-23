.PHONY: all clean

CC = g++
CFLAG = -std=c++11 -Wall
EXE = ../../bin/as
OBJ = main.o lexer.o symbol.o inst2code.o

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

%o: %cpp
	$(CC) $(CFLAG) -c $<

clean:
	$(shell rm -f *.o)