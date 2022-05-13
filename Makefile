all:
	$(MAKE) -C src/pc all

debug:
	./bin/pc -i test/fib.pas -V bin/tree.viz
	dot -Tpng bin/tree.viz -o bin/tree.png

clean:
	$(MAKE) -C src/pc clean