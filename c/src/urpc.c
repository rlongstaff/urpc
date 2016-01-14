
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

uint8_t urpc_accept(const urpc_stub *stub, urpc_connection *conn, urpc_frame *frame) {
	return stub->accept(conn, frame);

	/*
	 * TODO perform challenge-response hanshake
	 */
}

uint8_t urpc_connect(const urpc_stub *stub, urpc_endpoint *endpoint, urpc_connection *conn, urpc_frame *frame) {
	return stub->connect(endpoint, conn, frame);
	/*
	 * TODO perform challenge-response hanshake
	 */
}

uint8_t urpc_send(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame) {
	/*
	 * send client/session id
	 * maybe encrypt payload
	 * call stub->send
	 */
	stub->_send(conn, frame->rpc_buf.rpc.payload, frame->rpc_buf.rpc.header.payload_len);
	return URPC_SUCCESS;
}

uint8_t urpc_recv(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame) {
	/*
	 * recv 2 byte client/session id
	 * read 16 bytes
	 * maybe decrypt payload
	 * decode header
	 * read 16 byte chunks until msg->len, maybe decrypt
	 */
	stub->_recv(conn, frame->rpc_buf.rpc.payload, _URPC_MAX_PAYLOAD_SIZE);
	return URPC_SUCCESS;
}

uint8_t urpc_set_payload(urpc_rpc *rpc, const char *payload, uint16_t len) {
	memcpy(rpc->payload, payload, len);
	rpc->header.payload_len = len;
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
