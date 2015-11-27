

#include <stdio.h>
#include <stdlib.h>

#include "urpc.h"

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

    return 0;
}
