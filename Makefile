DEG_DIR = bin
SRC_DIR = test
DBG_SRC = $(SRC_DIR)/parser_decl_test.pas

.PHONY: all as util sim debug clean

all: pc

pc:
	$(MAKE) -C src/pc all

as:
	$(MAKE) -C src/as all

util:
	$(MAKE) -C src/util all

sim:
	$(MAKE) -C src/sim all

debug: all
	$(DEG_DIR)/pc -i $(DBG_SRC) -V $(DEG_DIR)/tree.viz
	dot -Tsvg -o $(DEG_DIR)/tree.svg $(DEG_DIR)/tree.viz
# $(DEG_DIR)/as -i $(DEG_DIR)/assembly.S -o $(DEG_DIR)/target.hex -s
# $(DEG_DIR)/hex2bin -i $(DEG_DIR)/target.hex -o $(DEG_DIR)/target.bin
# $(DEG_DIR)/rvsim $(DEG_DIR)/target.bin -s 0xFFFFFF -d

clean:
	$(MAKE) -C src/pc clean
	$(MAKE) -C src/as clean
	$(MAKE) -C src/util clean
	$(MAKE) -C src/sim clean