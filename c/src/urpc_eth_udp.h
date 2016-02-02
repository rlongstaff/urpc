/*
 * urpc_eth_udp.h
 *
 *  Created on: Dec 20, 2015
 *      Author: rob
 */

#ifndef URPC_ETH_UDP_H_
#define URPC_ETH_UDP_H_

/* TODO remove */
#include <stdio.h>

#include <netinet/in.h>

#include "urpc.h"

typedef struct urpc_endpoint_eth_udp {
    urpc_endpoint super;
    struct sockaddr_in addr;
    int fd;
} urpc_endpoint_eth_udp;

typedef struct urpc_connection_eth_udp {
    urpc_connection super;
    urpc_endpoint_eth_udp local;
    urpc_endpoint_eth_udp remote;
} urpc_connection_eth_udp;

uint8_t urpc_eth_udp_send(const urpc_connection* conn, const uint8_t* buf, uint16_t len);
uint8_t urpc_eth_udp_peek(const urpc_connection* conn, uint8_t* buf, uint16_t len);
uint8_t urpc_eth_udp_recv(const urpc_connection* conn, uint8_t* buf, uint16_t len);

extern const urpc_stub URPC_ETH_UDP_STUB;

#endif /* URPC_ETH_UDP_H_ */
