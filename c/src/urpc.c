

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urpc.h"

uint8_t urpc_init_client(const urpc_stub *stub) {
	return URPC_SUCCESS;
}

uint8_t urpc_connect(const urpc_stub *stub, const urpc_endpoint *endpoint, urpc_connection *conn) {
	return stub->connect(endpoint, conn);
}

uint8_t urpc_set_payload(urpc_message *msg, const char *payload) {
	strncpy(msg->payload, payload, _URPC_MAX_PAYLOAD_SIZE);
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
