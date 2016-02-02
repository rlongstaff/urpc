

INCLUDE=src/
INCLUDE_TEST=lib/cmocka/include

CFLAGS = -g

all: urpc_test urpc_test_eth_udp

tests: urpc_test urpc_test_eth_udp

%.o: src/%.c
	$(CC) $(CFLAGS) -I ${INCLUDE} -c -o bin/$@ $<

urpc_test.o: src/urpc_test.c
	$(CC) $(CFLAGS) -I ${INCLUDE} -I ${INCLUDE_TEST} -c -o bin/$@ $<

urpc_eth_udp_test.o: src/urpc_eth_udp_test.c
	$(CC) $(CFLAGS) -I ${INCLUDE} -I ${INCLUDE_TEST} -c -o bin/$@ $<

urpc_test: urpc.o urpc_test.o
	$(CC) $(CFLAGS) -I ${INCLUDE} -o bin/urpc_test \
		bin/urpc.o \
		bin/urpc_test.o \
		lib/cmocka/build/src/CMakeFiles/cmocka_shared.dir/cmocka.c.o

urpc_test_eth_udp: urpc.o urpc_client.o urpc_server.o urpc_eth_udp.o urpc_eth_udp_client.o urpc_eth_udp_server.o urpc_eth_udp_test.o
	$(CC) $(CFLAGS) -I ${INCLUDE} -o bin/urpc_eth_udp_test \
		bin/urpc.o \
		bin/urpc_client.o \
		bin/urpc_server.o \
		bin/urpc_eth_udp.o \
		bin/urpc_eth_udp_client.o \
		bin/urpc_eth_udp_server.o \
		bin/urpc_eth_udp_test.o \
		lib/cmocka/build/src/CMakeFiles/cmocka_shared.dir/cmocka.c.o
		
test: urpc_test urpc_test_eth_udp
	bin/urpc_test
	bin/urpc_eth_udp_test

clean:
	rm -f bin/*

