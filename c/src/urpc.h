/*
 * urpc.h
 *
 */

#ifndef URPC_H_
#define URPC_H_

#include <stdint.h>

#ifndef URPC_MAX_REQUEST_HANDLERS
#define URPC_MAX_REQUEST_HANDLERS  5
#endif /* URPC_MAX_REQUEST_HANDLERS */

#ifndef URPC_MAX_RESPONSE_HANDLERS
#define URPC_MAX_RESPONSE_HANDLERS 5
#endif /* URPC_MAX_RESPONSE_HANDLERS */

#define _URPC_CONTROL_REQUEST_HANDLERS 2
#define _URPC_CONTROL_RESPONSE_HANDLERS 2

/*
 * Block ciphers operate in set block sizes. The AES that is used here works
 * on 16 byte blocks. So that we can encrypt/decrypt in place, we keep the
 * frame size in a set multiple of chunks for the frame payload.
 */
#ifndef URPC_CHUNK_SIZE
#define URPC_CHUNK_SIZE 16
#endif /* URPC_CHUNK_SIZE */

/*
 * Allow users to override the max frame size by overriding the number of
 * chunks per frame. Default to a 64 byte frame payload.
 */
#ifndef URPC_MAX_FRAME_CHUNKS
#define URPC_MAX_FRAME_CHUNKS 4
#endif /* URPC_MAX_FRAME_CHUNKS */

#define _URPC_MAX_FRAME_SIZE URPC_MAX_FRAME_CHUNKS * URPC_CHUNK_SIZE + sizeof(struct urpc_frame_header)

#define _URPC_MAX_PAYLOAD_SIZE _URPC_MAX_FRAME_SIZE - sizeof(struct urpc_rpc_header) - sizeof(struct urpc_frame_header)
#define _URPC_CRC_POLY 0x96

#define URPC_ERROR 0
#define URPC_SUCCESS 1
#define URPC_INVALID_CRC 2
#define URPC_RPC_TOO_LARGE 3
#define URPC_INVALID_HANDLER 4
#define URPC_ERROR_UNKNOWN 255

#define URPC_RPC_FLAG_ERROR 1
#define URPC_RPC_FLAG_RESPONSE 2
#define URPC_RPC_FLAG_CONTROL 4

#define URPC_TRANSPORT_SERIAL 0
#define URPC_TRANSPORT_BLUETOOTH 1
#define URPC_TRANSPORT_ETH_UDP 2
#define URPC_TRANSPORT_ETH_TCP 3
#define URPC_TRANSPORT_WIFI_UDP 4
#define URPC_TRANSPORT_WIFI_TCP 5

typedef struct urpc_rpc_header {
    uint16_t payload_len;
    uint16_t request_id;
    uint8_t rpc_num;
    uint8_t flags;
    uint8_t payload_crc;
    uint8_t header_crc;
} urpc_rpc_header;

typedef struct urpc_frame_header {
    uint8_t version;
    uint8_t flags;
    uint32_t session;
} urpc_frame_header;

typedef struct urpc_rpc {
    urpc_rpc_header header;
    uint8_t payload[_URPC_MAX_PAYLOAD_SIZE];
} urpc_rpc;

typedef struct urpc_frame {
    urpc_frame_header header;
    urpc_rpc rpc;
} urpc_frame;

typedef union urpc_frame_buffer {
    urpc_frame frame;
    uint8_t _raw[_URPC_MAX_FRAME_SIZE];
} urpc_frame_buffer;

typedef struct urpc_endpoint {
} urpc_endpoint;

typedef struct urpc_connection {
    uint8_t transport;
    /* virtual local
     * virtual remote
     */
} urpc_connection;

typedef struct urpc_stub {
    uint8_t (*_send)(const urpc_connection* conn, const uint8_t* buf, uint16_t len);
    uint8_t (*_peek)(const urpc_connection* conn, uint8_t* buf, uint16_t len);
    uint8_t (*_recv)(const urpc_connection* conn, uint8_t* buf, uint16_t len);
} urpc_stub;

typedef void (*urpc_handler)(urpc_frame* frame);
uint8_t urpc_register_request_handler(urpc_handler handler, uint8_t rpc_id);
uint8_t urpc_register_response_handler(urpc_handler handler, uint8_t rpc_id);
uint8_t _urpc_register_control_request_handler(urpc_handler handler, uint8_t rpc_id);
uint8_t _urpc_register_control_response_handler(urpc_handler handler, uint8_t rpc_id);
void _urpc_clear_handlers(void);

uint8_t _urpc_handle_rpc(urpc_frame*);

uint8_t urpc_send(const urpc_stub* stub, const urpc_connection* conn, urpc_frame* frame);
uint8_t urpc_recv(const urpc_stub* stub, const urpc_connection* conn, urpc_frame* frame);

uint8_t urpc_set_payload(urpc_rpc* rpc, const char* payload, uint16_t len);

void print_frame(const urpc_frame* frame);

uint16_t _urpc_rpc_padded_size(uint16_t payload_length);
uint8_t _urpc_crc8(const uint8_t* msg, uint32_t msg_len);

#endif /* URPC_H_ */
