/*
 * urpc_eth_udp.c
 *
 *  Created on: Dec 20, 2015
 *      Author: rob
 */


#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "urpc_eth_udp.h"

uint8_t urpc_eth_udp_send(const urpc_connection* s_conn, const uint8_t* buf, uint16_t len) {
    urpc_connection_eth_udp* conn = (urpc_connection_eth_udp*)s_conn;

    if (sendto(conn->local.fd, buf, len, 0,
            (struct sockaddr*)&(conn->remote.addr),
            sizeof(struct sockaddr_in)) == -1) {
        printf("Oops:sendto\n");
    }

    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_peek(const urpc_connection* s_conn, uint8_t* buf, uint16_t len) {
    urpc_connection_eth_udp* conn = (urpc_connection_eth_udp*)s_conn;

    struct sockaddr_in address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    ssize_t recv_bytes = 0;
    recv_bytes = recvfrom(conn->local.fd,
            buf, len, MSG_PEEK, (struct sockaddr*)&address, &address_len);
    if (recv_bytes == -1) {
        return URPC_ERROR;
    }
    return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_recv(const urpc_connection* s_conn, uint8_t* buf, uint16_t len) {
    urpc_connection_eth_udp* conn = (urpc_connection_eth_udp*)s_conn;

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
