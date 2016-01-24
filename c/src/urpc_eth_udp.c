/*
 * urpc_eth_udp.c
 *
 *  Created on: Dec 20, 2015
 *      Author: rob
 */

/* TODO remove */
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "urpc_eth_udp.h"

/* Note that this would be PROGMEM for embedded platformds */
const urpc_stub URPC_ETH_UDP_STUB = {
    .init_client = &urpc_eth_udp_init_client,
    .init_server = &urpc_eth_udp_init_server,
    .accept      = &urpc_eth_udp_accept,
    .connect     = &urpc_eth_udp_connect,
    ._send       = &urpc_eth_udp_send,
    ._peek       = &urpc_eth_udp_peek,
    ._recv       = &urpc_eth_udp_recv,
};

const urpc_stub *urpc_eth_udp_get_stub(void) {
    return &URPC_ETH_UDP_STUB;
}

uint8_t urpc_eth_udp_init_client(urpc_client *client) {
    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_init_server(urpc_server *server, urpc_endpoint *s_endpoint) {
    urpc_endpoint_eth_udp *endpoint = (urpc_endpoint_eth_udp *)s_endpoint;

    endpoint->addr.sin_family = AF_INET;;
    endpoint->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (endpoint->fd == -1) {
        printf("Failed to create socket\n");
        return URPC_ERROR;
    }

    struct sockaddr_in *addr = &(endpoint->addr);
    if (bind(endpoint->fd, (struct sockaddr *)addr, sizeof(struct sockaddr_in)) == -1) {
        printf("Failed to bind: %s\n", strerror(errno));
        return URPC_ERROR;
    }
    server->endpoint = s_endpoint;

    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_accept(urpc_server *server, urpc_connection *conn, urpc_frame *frame) {

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

uint8_t urpc_eth_udp_send(const urpc_connection *s_conn, const uint8_t *buf, uint16_t len) {
    urpc_connection_eth_udp *conn = (urpc_connection_eth_udp *)s_conn;

    if (sendto(conn->local.fd, buf, len, 0,
            (struct sockaddr *)&(conn->remote.addr),
            sizeof(struct sockaddr_in)) == -1) {
        printf("Oops:sendto\n");
    }

    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_peek(const urpc_connection *s_conn, uint8_t *buf, uint16_t len) {
    urpc_connection_eth_udp *conn = (urpc_connection_eth_udp *)s_conn;

    struct sockaddr_in address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    ssize_t recv_bytes = 0;
    recv_bytes = recvfrom(conn->local.fd,
            buf, len, MSG_PEEK, (struct sockaddr *)&address, &address_len);
    if (recv_bytes == -1) {
        return URPC_ERROR;
    }
    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_recv(const urpc_connection *s_conn, uint8_t *buf, uint16_t len) {
    urpc_connection_eth_udp *conn = (urpc_connection_eth_udp *)s_conn;

    struct sockaddr_in address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    ssize_t recv_bytes = 0;

    while (recv_bytes < len) {
        recv_bytes = recvfrom(conn->local.fd,
                buf, len, 0, (struct sockaddr *)&address, &address_len);
        if (recv_bytes == -1) {
            return URPC_ERROR;
        }
        if (conn->remote.addr.sin_addr.s_addr == 0) {
            memcpy(&(conn->remote.addr), &address, sizeof(struct sockaddr_in));
        }
        buf += recv_bytes;
        len -= recv_bytes;
    }

    return URPC_SUCCESS;
}
