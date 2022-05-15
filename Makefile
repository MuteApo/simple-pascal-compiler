EXE = bin/pc
SRC = 

.PHONY: all debug clean

all:
	$(MAKE) -C src/pc all

debug: $(EXE)
	$(EXE) -i $(SRC) -V bin/tree.viz
	dot -Tpng bin/tree.viz -o bin/tree.png

clean:
	$(MAKE) -C src/pc clean