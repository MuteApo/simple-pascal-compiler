.PHONY: all clean

CC = g++
CFLAG = -std=c++11 -Wall
EXE = ../../bin/hex2bin
OBJ = hex2bin.o

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

%o: %cpp
	$(CC) $(CFLAG) -c $<

clean:
	$(shell rm -f *.o)