/*
 * urpc_server.h
 *
 *  Created on: Jan 30, 2016
 *      Author: rob
 */

#ifndef SRC_URPC_SERVER_H_
#define SRC_URPC_SERVER_H_

#include "urpc.h"

#ifndef URPC_MAX_CLIENTS
#define URPC_MAX_CLIENTS 8
#endif /* URPC_MAX_CLIENTS */

typedef struct urpc_server_conn {
    uint8_t state;
    urpc_connection *conn;
} urpc_con_slot;

typedef struct urpc_server {
    urpc_endpoint *endpoint;
    urpc_connection urpc_server_conn[URPC_MAX_CLIENTS];
} urpc_server;

typedef struct urpc_server_stub {
    uint8_t (*_send)(const urpc_connection *conn, const uint8_t *buf, uint16_t len);
    uint8_t (*_peek)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
    uint8_t (*_recv)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
    uint8_t (*init_server)(urpc_server *server, urpc_endpoint *endpoint);
    uint8_t (*accept)(urpc_server *server, urpc_connection *conn, urpc_frame *frame);
} urpc_server_stub;

uint8_t urpc_init_server(const urpc_server_stub *stub, urpc_server *server, urpc_endpoint *endpoint);
uint8_t urpc_accept(const urpc_server_stub *stub, urpc_server *server, urpc_connection *conn, urpc_frame *frame);

#endif /* SRC_URPC_SERVER_H_ */
