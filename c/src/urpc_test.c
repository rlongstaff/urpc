

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>

#include "cmocka.h"
#include "urpc.h"

static void test_crc(void **state) {
    (void) state; /* unused */
    uint8_t crc;
    char *msg = "abcdefghijklmnopqrstuvwxyz\0";
    char *msg2 = "zyxwvutsrqponmlkjihgfedcba\0";
    crc = _urpc_crc8((uint8_t *)msg, strlen(msg));
    assert_int_equal(crc, 0x90);
    crc = _urpc_crc8((uint8_t *)msg2, strlen(msg));
    assert_int_equal(crc, 0x96);
}

static void test_rpc_padding(void **state) {
    (void) state; /* unused */
    assert_int_equal(_urpc_rpc_padded_size(1), 16);
    assert_int_equal(_urpc_rpc_padded_size(8), 16);
    assert_int_equal(_urpc_rpc_padded_size(9), 32);
    assert_int_equal(_urpc_rpc_padded_size(40), 48);

}

int main (int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_crc),
        cmocka_unit_test(test_rpc_padding),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
