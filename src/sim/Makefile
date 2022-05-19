.PHONY: all clean

CC = g++
CFLAG = -std=c++11 -Wall
EXE = rvsim
OBJ = ram.o ecall.o register.o code2inst.o cpu.o main.o 

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

%o: %cpp
	$(CC) $(CFLAG) -c $<

clean:
	$(shell rm *.o $(EXE))