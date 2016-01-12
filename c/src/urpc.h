/*
 * urpc.h
 *
 */

#ifndef URPC_H_
#define URPC_H_

#ifndef URPC_MAX_MESSAGE_SIZE
#define URPC_MAX_MESSAGE_SIZE 64
#endif /* URPC_MAX_MESSAGE_SIZE */

#include <stdint.h>

#define _URPC_MAX_PAYLOAD_SIZE URPC_MAX_MESSAGE_SIZE - sizeof(struct urpc_header)
#define _URPC_CRC_POLY 0x96

#define URPC_ERROR 0
#define URPC_SUCCESS 1
#define URPC_ERROR_UNKNOWN 255

#define URPC_TRANSPORT_SERIAL 0
#define URPC_TRANSPORT_BLUETOOTH 1
#define URPC_TRANSPORT_ETH_UDP 2
#define URPC_TRANSPORT_ETH_TCP 3
#define URPC_TRANSPORT_WIFI_UDP 4
#define URPC_TRANSPORT_WIFI_TCP 5

typedef struct urpc_header {
    uint16_t payload_len;
    uint16_t request_id;
    uint8_t rpc_num;
    uint8_t flags;
    uint8_t reserved;
    uint8_t header_crc;
} urpc_header;

typedef struct urpc_message {
    urpc_header header;
    uint8_t payload[_URPC_MAX_PAYLOAD_SIZE];
} urpc_message;

typedef struct urpc_endpoint {
	uint8_t transport;
} urpc_endpoint;

typedef struct urpc_connection {
	uint32_t id;
} urpc_connection;

typedef struct urpc_stub {
	uint8_t (*init_client)(void);
	uint8_t (*init_server)(const urpc_endpoint *endpoint);
	uint8_t (*accept)(urpc_connection *conn);
	uint8_t (*connect)(urpc_endpoint *endpoint, urpc_connection *conn);
	uint8_t (*_send)(const urpc_connection *conn, const uint8_t *buf, uint16_t len);
	uint8_t (*_recv)(const urpc_connection *conn, uint8_t *buf, uint16_t len);
} urpc_stub;

uint8_t urpc_init_client(const urpc_stub *stub);
uint8_t urpc_init_server(const urpc_stub *stub, const urpc_endpoint *endpoint);
uint8_t urpc_accept(const urpc_stub *stub, urpc_connection *conn);
uint8_t urpc_connect(const urpc_stub *stub, urpc_endpoint *endpoint, urpc_connection *conn);
uint8_t urpc_send(const urpc_stub *stub, const urpc_connection *conn, const urpc_message *msg);
uint8_t urpc_recv(const urpc_stub *stub, const urpc_connection *conn, urpc_message *msg);

uint8_t urpc_set_payload(urpc_message *msg, const char *payload, uint16_t len);

uint8_t _urpc_crc8(const char *msg,
				   uint32_t msg_len,
				   const uint8_t poly);

#endif /* URPC_H_ */
