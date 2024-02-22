.PHONY: test build clean 
default: build
COMPILER_FLAGS := -Wno-int-conversion -Wall -pedantic

test: test/test.c src/progressbar.c src/progressbar.h
	@echo "Running tests"
	@mkdir -p build
	@gcc src/progressbar.c $(COMPILER_FLAGS) -c -Wl,-elib_init -fPIC -o build/progressbar.o
	@gcc -c test/test.c -o build/test.o
	@gcc build/progressbar.o build/test.o -lm -o build/test
	@rm build/*.o
	@./build/test

build: src/progressbar.c
	@mkdir -p build
	@gcc src/progressbar.c $(COMPILER_FLAGS) --shared -fPIC -lm -o build/progressbar.so

clean:
	-rm -r build
