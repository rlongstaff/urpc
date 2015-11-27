

INCLUDE=src/
INCLUDE_TEST=lib/cmocka/include

all: urpc_test

test: urpc_test

%.o: src/%.c
	gcc -I ${INCLUDE} -c -o bin/$@ $<

urpc_test.o: src/urpc_test.c
	gcc -I ${INCLUDE} -I ${INCLUDE_TEST} -c -o bin/$@ $<

urpc_test: urpc_test.o urpc.o
	gcc -I ${INCLUDE} -o bin/urpc_test \
		bin/urpc_test.o \
		bin/urpc.o \
		lib/cmocka/build/src/CMakeFiles/cmocka_shared.dir/cmocka.c.o
	bin/urpc_test

clean:
	rm -f bin/*.o bin/urpc_test

