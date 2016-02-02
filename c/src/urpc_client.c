/*
 * urpc_client.c
 *
 *  Created on: Feb 1, 2016
 *      Author: rob
 */

#include "urpc.h"
#include "urpc_client.h"

uint8_t urpc_init_client(const urpc_client_stub* stub, urpc_client* client) {
    return stub->init_client(client);
}

uint8_t urpc_connect(const urpc_client_stub* stub, urpc_client* client, urpc_connection* conn, urpc_frame* frame) {
    return stub->connect(client, conn, frame);
    /*
     * TODO perform challenge-response handshake.
     */
}
