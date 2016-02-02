/*
 * urpc_client_eth_udp.h
 *
 *  Created on: Jan 30, 2016
 *      Author: rob
 */

#ifndef SRC_URPC_ETH_UDP_CLIENT_H_
#define SRC_URPC_ETH_UDP_CLIENT_H_

#include "urpc.h"
#include "urpc_client.h"
#include "urpc_eth_udp.h"

const urpc_client_stub *urpc_eth_udp_get_client_stub(void);

uint8_t urpc_eth_udp_init_client(urpc_client *client);
uint8_t urpc_eth_udp_connect(urpc_client *client, urpc_connection *conn, urpc_frame *frame);

#endif /* SRC_URPC_ETH_UDP_CLIENT_H_ */
