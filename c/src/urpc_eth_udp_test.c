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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "cmocka.h"
#include "urpc.h"
#include "urpc_eth_udp.h"

static void test_urpc_connect(void **state) {
    (void) state; /* unused */
    urpc_client client;
    urpc_connection_eth_udp conn;
    urpc_frame frame;

    bzero(&client, sizeof(client));
    bzero(&frame, sizeof(urpc_frame));
    bzero(&conn, sizeof(urpc_connection_eth_udp));

    const urpc_stub *stub = urpc_eth_udp_get_stub();

    char *ip = "127.0.0.1\0";
    conn.remote.addr.sin_port = htons(31415);
    inet_aton(ip, &(conn.remote.addr.sin_addr));

    assert_int_equal(urpc_init_client(stub, &client), URPC_SUCCESS);

    uint8_t status = urpc_connect(stub, &client, (urpc_connection *)&conn, &frame);
    assert_int_equal(status, URPC_SUCCESS);
}

static void test_udp_send_recv(void **state) {
    (void) state; /* unused */
    urpc_client client;
    urpc_server server;
    urpc_frame client_frame, server_frame;
    urpc_connection_eth_udp client_conn, server_conn;
    urpc_endpoint_eth_udp client_endpoint, server_endpoint;

    bzero(&client, sizeof(client));
    bzero(&server, sizeof(server));
    bzero(&client_frame, sizeof(urpc_frame));
    bzero(&server_frame, sizeof(urpc_frame));
    bzero(&client_conn, sizeof(urpc_connection_eth_udp));
    bzero(&server_conn, sizeof(urpc_connection_eth_udp));

    const urpc_stub *stub = urpc_eth_udp_get_stub();

    char *ip = "127.0.0.1\0";
    int port = 31415;

    client_conn.remote.addr.sin_port = htons(port);
    inet_aton(ip, &(client_conn.remote.addr.sin_addr));

    server_conn.local.addr.sin_port = htons(port);
    inet_aton(ip, &(server_conn.local.addr.sin_addr));

    assert_int_equal(urpc_init_client(stub, &client), URPC_SUCCESS);
    assert_int_equal(urpc_init_server(stub, &server, (urpc_endpoint *)&server_conn.local), URPC_SUCCESS);

    uint8_t status = urpc_connect(stub, &client, (urpc_connection *)&client_conn, &client_frame);
    assert_int_equal(status, URPC_SUCCESS);

    status = urpc_accept(stub, &server, (urpc_connection *)&server_conn, &server_frame);

    char *payload = "abcdefghijklmnopqrstuvwxyz\0";
    urpc_set_payload(&(client_frame.rpc), payload, strlen(payload));

    assert_int_equal(urpc_send(stub, (urpc_connection *)&client_conn, &client_frame), URPC_SUCCESS);
    assert_int_equal(urpc_recv(stub, (urpc_connection *)&server_conn, &server_frame), URPC_SUCCESS);
    assert_string_equal(payload, &(server_frame.rpc.payload));
    bzero(&client_frame, sizeof(urpc_frame));
    assert_int_equal(urpc_send(stub, (urpc_connection *)&server_conn, &server_frame), URPC_SUCCESS);
    assert_int_equal(urpc_recv(stub, (urpc_connection *)&client_conn, &client_frame), URPC_SUCCESS);
    assert_string_equal(payload, &(client_frame.rpc.payload));
}

int main (int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_urpc_connect),
        cmocka_unit_test(test_udp_send_recv),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

