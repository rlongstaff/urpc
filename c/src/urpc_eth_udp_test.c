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

#include "cmocka.h"
#include "urpc.h"
#include "urpc_eth_udp.h"


static void test_urpc_connect(void **state) {
    (void) state; /* unused */
    urpc_stub stub;
    urpc_endpoint_eth_udp endpoint;
    endpoint.ip = 0;
    endpoint.port = 31415;

    urpc_eth_udp_get_stub(&stub);
    assert_int_equal(urpc_init_client(&stub), URPC_SUCCESS);
    urpc_connection conn;
    uint8_t status = urpc_connect(&stub, (urpc_endpoint *)&endpoint, &conn);
    assert_int_equal(status, URPC_SUCCESS);
    urpc_message msg;
    urpc_set_payload(&msg, "FOO\0");
    assert_int_equal(conn.send(&msg), URPC_SUCCESS);
}

int main (int argc, char **argv) {
    const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_urpc_connect),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

