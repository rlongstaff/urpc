/*
 * urpc.h
 *
 */

#ifndef URPC_H_
#define URPC_H_

#include <stdint.h>

/*
 * Block ciphers operate in set block sizes. The AES that is used here works
 * on 16 byte blocks. So that we can encrypt/decrypt in place, we keep the
 * frame size in a set multiple of chunks for the frame payload.
 */
#define _URPC_CHUNK_SIZE 16

/*
 * Allow users to override the max frame size by overriding the number of
 * chunks per frame. Default to a 64 byte frame payload.
 */
#ifndef URPC_MAX_FRAME_CHUNKS
#define URPC_MAX_FRAME_CHUNKS 4
#endif /* URPC_MAX_FRAME_CHUNKS */

#ifndef URPC_MAX_CLIENTS
#define URPC_MAX_CLIENTS 8
#endif /* URPC_MAX_CLIENTS */

#define _URPC_MAX_FRAME_SIZE URPC_MAX_FRAME_CHUNKS * _URPC_CHUNK_SIZE + sizeof(struct urpc_frame_header)

#define _URPC_MAX_PAYLOAD_SIZE _URPC_MAX_FRAME_SIZE - sizeof(struct urpc_rpc_header) - sizeof(struct urpc_frame_header)
#define _URPC_CRC_POLY 0x96

#define URPC_ERROR 0
#define URPC_SUCCESS 1
#define URPC_INVALID_CRC 2
#define URPC_RPC_TOO_LARGE 3
#define URPC_ERROR_UNKNOWN 255

#define URPC_FLAG_ERROR 1
#define URPC_FLAG_REPLY 2
#define URPC_FLAG_CONTROL 4

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

typedef struct urpc_client {
    urpc_connection *conn;
} urpc_client;

typedef struct urpc_server_conn {
    uint8_t state;
    urpc_connection *conn;
} urpc_con_slot;

typedef struct urpc_server {
    urpc_endpoint *endpoint;
    urpc_connection urpc_server_conn[URPC_MAX_CLIENTS];
} urpc_server;

typedef struct urpc_stub {
    uint8_t (*init_client)(urpc_client *client);
    uint8_t (*init_server)(urpc_server *server, urpc_endpoint *endpoint);
    uint8_t (*accept)(urpc_server *server, urpc_connection *conn, urpc_frame *frame);
    uint8_t (*connect)(urpc_client *client, urpc_connection *conn, urpc_frame *frame);
    uint8_t (*_send)(const urpc_connection *conn, const uint8_t *buf, uint16_t len);
    uint8_t (*_peek)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
    uint8_t (*_recv)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
} urpc_stub;

uint8_t urpc_init_client(const urpc_stub *stub, urpc_client *client);
uint8_t urpc_init_server(const urpc_stub *stub, urpc_server *server, urpc_endpoint *endpoint);
uint8_t urpc_accept(const urpc_stub *stub, urpc_server *server, urpc_connection *conn, urpc_frame *frame);
uint8_t urpc_connect(const urpc_stub *stub, urpc_client *client, urpc_connection *conn, urpc_frame *frame);
uint8_t urpc_send(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame);
uint8_t urpc_recv(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame);

uint8_t urpc_set_payload(urpc_rpc *rpc, const char *payload, uint16_t len);

void _print_frame(const urpc_frame *frame);

void _urpc_set_flag(urpc_rpc_header *rpc, uint8_t flags);
void _urpc_clear_flag(urpc_rpc_header *rpc, uint8_t flags);
uint16_t _urpc_rpc_padded_size(uint16_t payload_length);
uint8_t _urpc_crc8(const uint8_t *msg, uint32_t msg_len);

#endif /* URPC_H_ */
