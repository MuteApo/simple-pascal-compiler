DBG_EXE = bin/pc
DBG_SRC = test/parser_decl_test.pas
DBG_VIZ_PREFIX = bin/tree

.PHONY: all sim debug clean

all:
	$(MAKE) -C src/pc all

sim:
	$(MAKE) -C src/sim all

util:
	$(MAKE) -C src/util all

debug: all sim
	$(DBG_EXE) -i $(DBG_SRC) -V $(DBG_VIZ_PREFIX).viz
	dot -Tpng -o $(DBG_VIZ_PREFIX).png $(DBG_VIZ_PREFIX).viz

clean:
	$(MAKE) -C src/pc clean
	$(MAKE) -C src/sim clean
	$(MAKE) -C src/util clean