

INCLUDE=src/

all: urpc_test

%.o: src/%.c
	gcc -I ${INCLUDE} -c -o bin/$@ $<

urpc_test: urpc_test.o urpc.o
	gcc -I ${INCLUDE} -o bin/urpc_test bin/urpc_test.o bin/urpc.o

clean:
	rm -f bin/*.o bin/urpc_test

