BIN_DIR = bin
PAS_DIR = test
PAS_SRC = $(PAS_DIR)/array_record_nest.pas

.PHONY: pc as util sim build clean compile run debug visual all

pc:
	$(MAKE) -C src/pc all

as:
	$(MAKE) -C src/as all

util:
	$(MAKE) -C src/util all

sim:
	$(MAKE) -C src/sim all

build: pc as util sim

clean:
	$(MAKE) -C src/pc clean
	$(MAKE) -C src/as clean
	$(MAKE) -C src/util clean
	$(MAKE) -C src/sim clean

visual: build
	$(BIN_DIR)/pc -i $(PAS_SRC) -V $(BIN_DIR)/tree_run0.viz $(BIN_DIR)/tree_run1.viz
	dot -Tsvg -o $(BIN_DIR)/tree_run0.svg $(BIN_DIR)/tree_run0.viz
	dot -Tsvg -o $(BIN_DIR)/tree_run1.svg $(BIN_DIR)/tree_run1.viz

compile: build
	$(BIN_DIR)/pc -i $(PAS_SRC) -o $(BIN_DIR)/assembly.S
	$(BIN_DIR)/as -i $(BIN_DIR)/assembly.S -o $(BIN_DIR)/target.hex -s
	$(BIN_DIR)/hex2bin -i $(BIN_DIR)/target.hex -o $(BIN_DIR)/target.bin

run: compile
	$(BIN_DIR)/rvsim $(BIN_DIR)/target.bin -s 0xFFFFFF

debug: compile
	$(BIN_DIR)/rvsim $(BIN_DIR)/target.bin -s 0xFFFFFF -d

all: compile run