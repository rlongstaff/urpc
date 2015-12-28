

INCLUDE=src/
INCLUDE_TEST=lib/cmocka/include

all: urpc_test urpc_test_eth_udp

test: urpc_test urpc_test_eth_udp

%.o: src/%.c
	gcc -I ${INCLUDE} -c -o bin/$@ $<

urpc_test.o: src/urpc_test.c
	gcc -I ${INCLUDE} -I ${INCLUDE_TEST} -c -o bin/$@ $<

urpc_eth_udp_test.o: src/urpc_eth_udp_test.c
	gcc -I ${INCLUDE} -I ${INCLUDE_TEST} -c -o bin/$@ $<

urpc_test: urpc.o urpc_test.o
	gcc -I ${INCLUDE} -o bin/urpc_test \
		bin/urpc.o \
		bin/urpc_test.o \
		lib/cmocka/build/src/CMakeFiles/cmocka_shared.dir/cmocka.c.o
	bin/urpc_test

urpc_test_eth_udp: urpc.o urpc_eth_udp.o urpc_eth_udp_test.o
	gcc -I ${INCLUDE} -o bin/urpc_eth_udp_test \
		bin/urpc.o \
		bin/urpc_eth_udp.o \
		bin/urpc_eth_udp_test.o \
		lib/cmocka/build/src/CMakeFiles/cmocka_shared.dir/cmocka.c.o
	bin/urpc_eth_udp_test

clean:
	rm -f bin/*

