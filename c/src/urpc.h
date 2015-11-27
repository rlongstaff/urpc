/*
 * urpc.h
 *
 */

#ifndef _URPC_H
#define _URPC_H

#ifndef _URPC_MAX_PAYLOAD_SIZE
#define _URPC_MAX_PAYLOAD_SIZE 56
#endif /* _URPC_MAX_PAYLOAD_SIZE */

struct urpc_header {
    uint16_t payload_len;
    uint16_t request_id;
    uint8_t rpc_num;
    uint8_t flags;
    uint8_t reserved;
    uint8_t header_crc;
};

struct urpc_message {
    struct urpc_header header;
    uint8_t payload[_URPC_MAX_PAYLOAD_SIZE];
};

#define _URPC_MAX_MESSAGE_SIZE _URPC_MAX_PAYLOAD_SIZE + sizeof(struct urpc_header)

#define _URPC_CRC_POLY 0x96

uint8_t _urpc_crc8(const char *msg,
					uint32_t msg_len,
					const uint8_t poly);

#endif /* _URPC_H */
