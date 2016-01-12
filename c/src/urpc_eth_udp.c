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
	._recv       = &urpc_eth_udp_recv,
};

const urpc_stub *urpc_eth_udp_get_stub(void) {
	return &URPC_ETH_UDP_STUB;
}

uint8_t urpc_eth_udp_init_client(void) {
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_init_server(const urpc_endpoint *endpoint) {
	((urpc_endpoint_eth_udp *)endpoint)->super.transport = URPC_TRANSPORT_ETH_UDP;

	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_accept(urpc_connection *conn) {
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_connect(urpc_endpoint *s_endpoint, urpc_connection *s_conn) {
	urpc_connection_eth_udp *conn = (urpc_connection_eth_udp *)s_conn;
	urpc_endpoint_eth_udp *endpoint = (urpc_endpoint_eth_udp *)s_endpoint;

	endpoint->super.transport = URPC_TRANSPORT_ETH_UDP;

	bzero(conn, sizeof(urpc_connection_eth_udp));
	conn->addr.sin_family = AF_INET;
	conn->fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (conn->fd == -1) {
	    printf("Failed to create socket\n");
	    return URPC_ERROR;
	}

	if (inet_aton(endpoint->ip, &(conn->addr.sin_addr)) == 0) {
		printf("Failed to parse IP: %s\n", endpoint->ip);
		return URPC_ERROR;
	}
	conn->addr.sin_port = htons(endpoint->port);
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_send(const urpc_connection *s_conn, const uint8_t *buf, uint16_t len) {
	urpc_connection_eth_udp *conn = (urpc_connection_eth_udp *)s_conn;

	if (sendto(conn->fd, buf, len, 0,
			(struct sockaddr *)&(conn->addr),
			sizeof(struct sockaddr_in)) == -1) {
	    printf("Oops:sendto\n");
	}

	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_recv(const urpc_connection *conn, uint8_t *buf, uint16_t len) {
	return URPC_SUCCESS;
}
