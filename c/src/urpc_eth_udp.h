/*
 * urpc_eth_udp.h
 *
 *  Created on: Dec 20, 2015
 *      Author: rob
 */

#ifndef URPC_ETH_UDP_H_
#define URPC_ETH_UDP_H_

#include "urpc.h"

typedef struct urpc_endpoint_eth_udp {
	urpc_endpoint super;
	uint32_t ip;
	int16_t port;
} urpc_endpoint_eth_udp;


uint8_t urpc_eth_udp_get_stub(urpc_stub *stub);
uint8_t urpc_eth_udp_init_client(void);
uint8_t urpc_eth_udp_init_server(const urpc_endpoint *endpoint);
uint8_t urpc_eth_udp_accept(urpc_connection *conn);
uint8_t urpc_eth_udp_connect(const urpc_endpoint *endpoint, urpc_connection *conn);
uint8_t urpc_eth_udp_send(const urpc_connection *conn, const urpc_message *msg);
uint8_t urpc_eth_udp_recv(const urpc_connection *conn, urpc_message *msg);


#endif /* URPC_ETH_UDP_H_ */
