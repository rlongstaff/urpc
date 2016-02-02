/*
 * urpc_server_eth_udp.c
 *
 *  Created on: Jan 30, 2016
 *      Author: rob
 */

#include <errno.h>
#include <string.h>
#include <sys/socket.h>

#include "urpc.h"
#include "urpc_eth_udp.h"
#include "urpc_eth_udp_server.h"

/* Note that this would be PROGMEM for embedded platformds */
const urpc_server_stub URPC_ETH_UDP_SERVER_STUB = {
    ._send       = &urpc_eth_udp_send,
    ._peek       = &urpc_eth_udp_peek,
    ._recv       = &urpc_eth_udp_recv,
    .init_server = &urpc_eth_udp_init_server,
    .accept      = &urpc_eth_udp_accept,
};

const urpc_server_stub* urpc_eth_udp_get_server_stub(void) {
    return &URPC_ETH_UDP_SERVER_STUB;
}

uint8_t urpc_eth_udp_init_server(urpc_server* server, urpc_endpoint* s_endpoint) {
    urpc_endpoint_eth_udp* endpoint = (urpc_endpoint_eth_udp*)s_endpoint;

    endpoint->addr.sin_family = AF_INET;;
    endpoint->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (endpoint->fd == -1) {
        printf("Failed to create socket\n");
        return URPC_ERROR;
    }

    struct sockaddr_in* addr = &(endpoint->addr);
    if (bind(endpoint->fd, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) == -1) {
        printf("Failed to bind: %s\n", strerror(errno));
        return URPC_ERROR;
    }
    server->endpoint = s_endpoint;

    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_accept(urpc_server* server, urpc_connection* conn, urpc_frame* frame) {

    return URPC_SUCCESS;
}

