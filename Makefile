DBG_EXE = bin/pc
DBG_SRC = test/parser_decl_test.pas
DBG_VIZ_PREFIX = bin/tree

.PHONY: all debug clean

all: src/pc all
	$(MAKE) -C src/pc all

debug: all
	$(DBG_EXE) -i $(DBG_SRC) -V $(DBG_VIZ_PREFIX).viz
	dot -Tpng -o $(DBG_VIZ_PREFIX).png $(DBG_VIZ_PREFIX).viz

clean:
	$(MAKE) -C src/pc clean