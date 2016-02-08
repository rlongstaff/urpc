

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>

#include "cmocka.h"
#include "urpc.h"

static void test_crc(void** state) {
    (void) state; /* unused */
    uint8_t crc;
    char *msg = "abcdefghijklmnopqrstuvwxyz\0";
    char *msg2 = "zyxwvutsrqponmlkjihgfedcba\0";
    crc = _urpc_crc8((uint8_t *)msg, strlen(msg));
    assert_int_equal(crc, 0x90);
    crc = _urpc_crc8((uint8_t *)msg2, strlen(msg));
    assert_int_equal(crc, 0x96);
}

static void test_rpc_padding(void** state) {
    (void) state; /* unused */
    assert_int_equal(_urpc_rpc_padded_size(1), 16);
    assert_int_equal(_urpc_rpc_padded_size(8), 16);
    assert_int_equal(_urpc_rpc_padded_size(9), 32);
    assert_int_equal(_urpc_rpc_padded_size(40), 48);

}

uint8_t req_widget = 0;
uint8_t resp_widget = 0;
uint8_t con_req_widget = 0;
uint8_t con_resp_widget = 0;

void req_handler(urpc_frame* frame) {
    req_widget = frame->rpc.payload[0];
}
void resp_handler(urpc_frame* frame) {
    resp_widget = frame->rpc.payload[0];
}
void con_req_handler(urpc_frame* frame) {
    con_req_widget = frame->rpc.payload[0];
}
void con_resp_handler(urpc_frame* frame) {
    con_resp_widget = frame->rpc.payload[0];
}

static void test_req_handler(void** state) {
    (void) state; /* unused */
    urpc_register_request_handler(req_handler, 0);
    urpc_frame frame;
    bzero(&frame, sizeof(urpc_frame));
    frame.rpc.payload[0] = 1;
    _urpc_handle_rpc(&frame);
    assert_int_equal(frame.rpc.payload[0], req_widget);
}

static void test_resp_handler(void** state) {
    (void) state; /* unused */
    urpc_register_response_handler(resp_handler, 0);
    urpc_frame frame;
    bzero(&frame, sizeof(urpc_frame));
    frame.rpc.header.flags |= URPC_RPC_FLAG_RESPONSE;
    frame.rpc.payload[0] = 1;
    _urpc_handle_rpc(&frame);
    assert_int_equal(frame.rpc.payload[0], resp_widget);
}

static void test_con_req_handler(void** state) {
    (void) state; /* unused */
    _urpc_register_control_request_handler(con_req_handler, 0);
    urpc_frame frame;
    bzero(&frame, sizeof(urpc_frame));
    frame.rpc.header.flags |= URPC_RPC_FLAG_CONTROL;
    frame.rpc.payload[0] = 1;
    _urpc_handle_rpc(&frame);
    assert_int_equal(frame.rpc.payload[0], con_req_widget);

}

static void test_con_resp_handler(void** state) {
    (void) state; /* unused */
    _urpc_register_control_response_handler(con_resp_handler, 0);
    urpc_frame frame;
    bzero(&frame, sizeof(urpc_frame));
    frame.rpc.header.flags |= URPC_RPC_FLAG_CONTROL | URPC_RPC_FLAG_RESPONSE;
    frame.rpc.payload[0] = 1;
    _urpc_handle_rpc(&frame);
    assert_int_equal(frame.rpc.payload[0], con_resp_widget);
}

static void test_invalid_req_handler(void** state) {
    (void) state; /* unused */
    _urpc_clear_handlers();
    urpc_frame frame;
    bzero(&frame, sizeof(urpc_frame));
    frame.rpc.payload[0] = 1;
    req_widget = 0;
    _urpc_handle_rpc(&frame);
    assert_int_not_equal(frame.rpc.payload[0], req_widget);
}

int main (int argc, char** argv) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_crc),
            cmocka_unit_test(test_rpc_padding),
            cmocka_unit_test(test_req_handler),
            cmocka_unit_test(test_resp_handler),
            cmocka_unit_test(test_con_req_handler),
            cmocka_unit_test(test_con_resp_handler),
            cmocka_unit_test(test_invalid_req_handler),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
