
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
     * TODO perform challenge-response handshake
     */
}

uint8_t urpc_connect(const urpc_stub *stub, urpc_endpoint *endpoint, urpc_connection *conn, urpc_frame *frame) {
    return stub->connect(endpoint, conn, frame);
    /*
     * TODO perform challenge-response handshake.
     */
}

uint8_t urpc_send(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame) {
    uint16_t payload_length = frame->rpc.header.payload_len;

    frame->header.session = htonl(frame->header.session);
    frame->rpc.header.payload_len = htons(frame->rpc.header.payload_len);
    frame->rpc.header.request_id = htons(frame->rpc.header.request_id);
    frame->rpc.header.header_crc = _urpc_crc8((uint8_t *)&(frame->rpc.header),
            sizeof(urpc_rpc_header) - sizeof(uint8_t));
    stub->_send(conn, ((urpc_frame_buffer *)frame)->_raw,
            sizeof(urpc_frame_header) +    _urpc_rpc_padded_size(payload_length));
    return URPC_SUCCESS;
}

uint8_t urpc_recv(const urpc_stub *stub, const urpc_connection *conn, urpc_frame *frame) {
    /* The _recv stub will block until it reads a full message. To know exactly
     * how much of a message to wait for, we peek at the first chunk for the
     * payload length. We then pull the whole frame off the wire and splice the
     * first chunk over the beginning of the frame to avoid decrypting the same
     * data twice.
     */

    uint8_t status = URPC_SUCCESS;

    uint8_t peek_buf[sizeof(urpc_frame_header) + _URPC_CHUNK_SIZE];
    urpc_frame *peek_frame;

    bzero(peek_buf, sizeof(urpc_frame_header) + _URPC_CHUNK_SIZE);

    stub->_peek(conn, peek_buf, sizeof(urpc_frame_header) + _URPC_CHUNK_SIZE);
    peek_frame = (urpc_frame *) peek_buf;
    peek_frame->header.session = ntohl(peek_frame->header.session);

    /* TODO Lookup encryption key */
    /* TODO decrypt chunk*/

    /* Before we do anything else, let's make sure our stream is aligned and
     * the header is valid.
     */
    if(peek_frame->rpc.header.header_crc !=
            _urpc_crc8((uint8_t *)&(peek_frame->rpc.header),
                    sizeof(urpc_rpc_header) - sizeof(uint8_t))) {
        /* Bzzzt. This isn't a good header. Flush the socket and walk away. */
        stub->_recv(conn, ((urpc_frame_buffer *)frame)->_raw, 0);
        return URPC_INVALID_CRC;
    }

    peek_frame->rpc.header.request_id = ntohs(peek_frame->rpc.header.request_id);
    peek_frame->rpc.header.payload_len = ntohs(peek_frame->rpc.header.payload_len);

    if (peek_frame->rpc.header.payload_len > _URPC_MAX_PAYLOAD_SIZE) {
        peek_frame->rpc.header.payload_len = _URPC_MAX_PAYLOAD_SIZE;
        status = URPC_RPC_TOO_LARGE;
    }

    /* We have a good header and we are ready to pull in the entire message. */
    bzero(frame, sizeof(urpc_frame));
    stub->_recv(conn, ((urpc_frame_buffer *)frame)->_raw,
            sizeof(urpc_frame_header) + _urpc_rpc_padded_size(peek_frame->rpc.header.payload_len));
    /* Copy our already-decrypted initial chunk into the destination frame. */
    memcpy(((urpc_frame_buffer *)frame)->_raw, peek_buf, sizeof(peek_buf));

    /* TODO decrypt remaining chunks*/
    return status;
}

uint8_t urpc_set_payload(urpc_rpc *rpc, const char *payload, uint16_t len) {
    if (len > _URPC_MAX_PAYLOAD_SIZE) {
        return URPC_RPC_TOO_LARGE;
    }
    memcpy(rpc->payload, payload, len);
    rpc->header.payload_len = len;
    return URPC_SUCCESS;
}

void print_frame(const urpc_frame *frame) {
    printf("============\n");
    printf("Session:  %d\n", frame->header.session);
    printf("Request:  %d\n", frame->rpc.header.request_id);
    printf("RPC:      %d\n", frame->rpc.header.rpc_num);
    printf("Flags:    %d\n", frame->rpc.header.flags);
    printf("Reserved: %d\n", frame->rpc.header.reserved);
    printf("Length:   %d\n", frame->rpc.header.payload_len);
    printf("CRC:      %d\n", frame->rpc.header.header_crc);
    printf("============\n");
}

inline void _urpc_set_flag(urpc_rpc_header *rpc, uint8_t flags) {
    rpc->flags |= flags;
}
inline void _urpc_clear_flag(urpc_rpc_header *rpc, uint8_t flags) {
    rpc->flags &= ~flags;
}

inline uint16_t _urpc_rpc_padded_size(uint16_t payload_length) {
    if ((sizeof(urpc_rpc_header) + payload_length) % _URPC_CHUNK_SIZE) {
        return _URPC_CHUNK_SIZE -
                ((sizeof(urpc_rpc_header) + payload_length) % _URPC_CHUNK_SIZE) +
                (sizeof(urpc_rpc_header) + payload_length);
    }
    return (sizeof(urpc_rpc_header) + payload_length);
}

inline uint8_t _urpc_crc8(const uint8_t *msg,
                    uint32_t msg_len) {
    uint8_t crc = 0x00;
    for (uint32_t i = 0; i < msg_len; i++) {
        crc ^= msg[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ _URPC_CRC_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
