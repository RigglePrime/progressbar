default := build

test: test/test.c src/progressbar.c src/progressbar.h
	@echo "Running tests"
	mkdir -p build
	gcc src/progressbar.c -Wno-int-conversion -c -Wl,-elib_init -fPIC -o build/progressbar.o
	gcc -c test/test.c -o build/test.o
	gcc build/progressbar.o build/test.o -lm -o build/test
	rm build/*.o
	./build/test

build: src/progressbar.c
	mkdir -p build
	gcc src/progressbar.c -Wno-int-conversion -shared -fPIC -lm -o build/progressbar.so

clean:
	-rm -r build
