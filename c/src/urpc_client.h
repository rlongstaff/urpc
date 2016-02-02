/*
 * urpc_client.h
 *
 *  Created on: Feb 1, 2016
 *      Author: rob
 */

#ifndef SRC_URPC_CLIENT_H_
#define SRC_URPC_CLIENT_H_

#include "urpc.h"

typedef struct urpc_client {
    urpc_connection *conn;
} urpc_client;

typedef struct urpc_client_stub {
    uint8_t (*_send)(const urpc_connection *conn, const uint8_t *buf, uint16_t len);
    uint8_t (*_peek)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
    uint8_t (*_recv)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
    uint8_t (*init_client)(urpc_client *client);
    uint8_t (*connect)(urpc_client *client, urpc_connection *conn, urpc_frame *frame);
} urpc_client_stub;

uint8_t urpc_init_client(const urpc_client_stub *stub, urpc_client *client);
uint8_t urpc_connect(const urpc_client_stub *stub, urpc_client *client, urpc_connection *conn, urpc_frame *frame);

#endif /* SRC_URPC_CLIENT_H_ */
