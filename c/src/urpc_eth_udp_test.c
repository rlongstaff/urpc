/*
 * urpc_eth_udp_test.c
 *
 *  Created on: Dec 27, 2015
 *      Author: rob
 */



#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "cmocka.h"
#include "urpc.h"
#include "urpc_eth_udp.h"

static void test_urpc_connect(void **state) {
    (void) state; /* unused */
    urpc_endpoint_eth_udp endpoint;
    urpc_connection_eth_udp conn;
    urpc_frame frame;

    const urpc_stub *stub = urpc_eth_udp_get_stub();

    char *ip = "127.0.0.1\0";
    strncpy(endpoint.ip, ip, strlen(ip));
    endpoint.port = 31415;

    assert_int_equal(urpc_init_client(stub), URPC_SUCCESS);

    uint8_t status = urpc_connect(stub, (urpc_endpoint *)&endpoint, (urpc_connection *)&conn, &frame);
    assert_int_equal(status, URPC_SUCCESS);
}

static void test_udp_echo(void **state) {
    urpc_endpoint_eth_udp endpoint;
    urpc_connection_eth_udp conn;
    urpc_frame frame;

    const urpc_stub *stub = urpc_eth_udp_get_stub();

    bzero(&frame, sizeof(urpc_frame));

    char *ip = "127.0.0.1\0";
    strncpy(endpoint.ip, ip, strlen(ip));
    endpoint.port = 31415;

    assert_int_equal(urpc_init_client(stub), URPC_SUCCESS);

    uint8_t status = urpc_connect(stub, (urpc_endpoint *)&endpoint, (urpc_connection *)&conn, &frame);
    assert_int_equal(status, URPC_SUCCESS);

    char *payload = "abcdefghijklmnopqrstuvwxyz\0";
    urpc_set_payload(&(frame.rpc), payload, strlen(payload));

    assert_int_equal(urpc_send(stub, (urpc_connection *)&conn, &frame), URPC_SUCCESS);
    bzero(&frame, sizeof(urpc_frame));
    assert_int_equal(urpc_recv(stub, (urpc_connection *)&conn, &frame), URPC_SUCCESS);
}


int main (int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_urpc_connect),
        cmocka_unit_test(test_udp_echo),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

