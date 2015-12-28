/*
 * urpc_eth_udp.c
 *
 *  Created on: Dec 20, 2015
 *      Author: rob
 */

/* TODO remove */
#include <stdio.h>

#include "urpc_eth_udp.h"


uint8_t  urpc_eth_udp_get_stub(urpc_stub *stub) {
	stub->init_client = &urpc_eth_udp_init_client;
	stub->init_server = &urpc_eth_udp_init_server;
	stub->accept      = &urpc_eth_udp_accept;
	stub->connect     = &urpc_eth_udp_connect;
	stub->send        = &urpc_eth_udp_send;
	stub->recv        = &urpc_eth_udp_recv;
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_init_client(void) {
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_init_server(const urpc_endpoint *endpoint) {
	urpc_endpoint_eth_udp *ep = (urpc_endpoint_eth_udp *)endpoint;
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_accept(urpc_connection *conn) {
	conn->recv = &urpc_eth_udp_recv;
	conn->send = &urpc_eth_udp_send;
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_connect(const urpc_endpoint *endpoint, urpc_connection *conn) {
	conn->recv = &urpc_eth_udp_recv;
	conn->send = &urpc_eth_udp_send;
	urpc_endpoint_eth_udp *ep = (urpc_endpoint_eth_udp *)endpoint;
	printf("port: %d", ep->port);
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_send(const urpc_connection *conn, const urpc_message *msg) {
	return URPC_SUCCESS;
}

uint8_t urpc_eth_udp_recv(const urpc_connection *conn, urpc_message *msg) {
	printf("ETH UDP recv");
	return URPC_SUCCESS;
}
