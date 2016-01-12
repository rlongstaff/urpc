
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urpc.h"

uint8_t urpc_init_client(const urpc_stub *stub) {
	return stub->init_client();
}

uint8_t urpc_init_server(const urpc_stub *stub, const urpc_endpoint *endpoint) {
	return stub->init_server(endpoint);
}

uint8_t urpc_accept(const urpc_stub *stub, urpc_connection *conn) {
	return stub->accept(conn);
}
uint8_t urpc_connect(const urpc_stub *stub, urpc_endpoint *endpoint, urpc_connection *conn) {
	return stub->connect(endpoint, conn);
}

uint8_t urpc_send(const urpc_stub *stub, const urpc_connection *conn, const urpc_message *msg) {
	/*
	 * send client/session id
	 * maybe encrypt payload
	 * call stub->send
	 */
	stub->_send(conn, msg->payload, msg->header.payload_len);
	return URPC_SUCCESS;
}

uint8_t urpc_recv(const urpc_stub *stub, const urpc_connection *conn, urpc_message *msg) {
	/*
	 * recv 2 byte client/session id
	 * read 16 bytes
	 * maybe decrypt payload
	 * decode header
	 * read 16 byte chunks until msg->len, maybe decrypt
	 */
	stub->_recv(conn, msg->payload, _URPC_MAX_PAYLOAD_SIZE);
	return URPC_SUCCESS;
}

uint8_t urpc_set_payload(urpc_message *msg, const char *payload, uint16_t len) {
	memcpy(msg->payload, payload, len);
	msg->header.payload_len = len;
	return URPC_SUCCESS;
}

uint8_t _urpc_crc8(const char *msg,
					uint32_t msg_len,
					const uint8_t poly) {
	uint8_t crc = 0x00;
    for (uint32_t i = 0; i < msg_len; i++) {
    	crc ^= msg[i];
    	for (int j = 0; j < 8; j++) {
    		if (crc & 0x80) {
    			crc = (crc << 1) ^ poly;
    		} else {
    			crc <<= 1;
    		}
    	}
    }
    return crc;
}
