/*
 * urpc_eth_udp_client.c
 *
 *  Created on: Jan 31, 2016
 *      Author: rob
 */

#include "urpc.h"
#include "urpc_eth_udp.h"
#include "urpc_eth_udp_client.h"

const urpc_client_stub URPC_ETH_UDP_CLIENT_STUB = {
    ._send       = &urpc_eth_udp_send,
    ._peek       = &urpc_eth_udp_peek,
    ._recv       = &urpc_eth_udp_recv,
    .init_client = &urpc_eth_udp_init_client,
    .connect     = &urpc_eth_udp_connect,
};

const urpc_client_stub *urpc_eth_udp_get_client_stub(void) {
    return &URPC_ETH_UDP_CLIENT_STUB;
}

uint8_t urpc_eth_udp_init_client(urpc_client *client) {
    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_connect(urpc_client *client, urpc_connection *conn, urpc_frame *frame) {

    ((urpc_connection_eth_udp *)conn)->super.transport = URPC_TRANSPORT_ETH_UDP;
    ((urpc_connection_eth_udp *)conn)->remote.addr.sin_family = AF_INET;
    ((urpc_connection_eth_udp *)conn)->local.fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (((urpc_connection_eth_udp *)conn)->local.fd == -1) {
        printf("Failed to create socket\n");
        return URPC_ERROR;
    }

    client->conn = conn;

    return URPC_SUCCESS;
}
