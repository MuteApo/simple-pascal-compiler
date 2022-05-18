.PHONY: all clean

CC = g++
EXE = rvsim
OBJ = ram.o ecall.o register.o code2inst.o cpu.o main.o 

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

%o: %cpp
	$(CC) -c $<

clean:
	$(shell rm *.o $(EXE))