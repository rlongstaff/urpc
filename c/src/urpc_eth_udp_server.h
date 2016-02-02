/*
 * urpc_eth_udp_server.h
 *
 *  Created on: Jan 31, 2016
 *      Author: rob
 */

#ifndef SRC_URPC_ETH_UDP_SERVER_H_
#define SRC_URPC_ETH_UDP_SERVER_H_

#include "urpc.h"
#include "urpc_server.h"
#include "urpc_eth_udp.h"

const urpc_server_stub *urpc_eth_udp_get_server_stub(void);

uint8_t urpc_eth_udp_init_server(urpc_server *server, urpc_endpoint *endpoint);
uint8_t urpc_eth_udp_accept(urpc_server *server, urpc_connection *conn, urpc_frame *frame);


#endif /* SRC_URPC_ETH_UDP_SERVER_H_ */
