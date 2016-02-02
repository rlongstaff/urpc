/*
 * urpc_server.c
 *
 *  Created on: Jan 30, 2016
 *      Author: rob
 */

#include "urpc_server.h"

uint8_t urpc_init_server(const urpc_server_stub *stub, urpc_server *server, urpc_endpoint *endpoint) {
    return stub->init_server(server, endpoint);
}

uint8_t urpc_accept(const urpc_server_stub *stub, urpc_server *server, urpc_connection *conn, urpc_frame *frame) {
    return stub->accept(server, conn, frame);

    /*
     * TODO perform challenge-response handshake
     */
}
