/*
 * urpc.h
 *
 */

#ifndef _URPC_H
#define _URPC_H

#define MAX_PAYLOAD_SIZE 56

struct urpc_header {
    unsigned short int request_id;
    unsigned char rpc_num;
    unsigned char flags;
    unsigned short int payload_len;
    unsigned char reserved;
    unsigned char header_crc;
};

struct urpc_message {
    struct urpc_header header;
    unsigned char payload[MAX_PAYLOAD_SIZE];
};

#define MAX_MESSAGE_SIZE MAX_PAYLOAD_SIZE + sizeof(struct urpc_header) + sizeof(unsigned char)

#endif /* _URPC_H */
