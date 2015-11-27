

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>

#include "cmocka.h"
#include "urpc.h"

static void test_message_crc(void **state) {
    (void) state; /* unused */
    uint8_t crc;
    char *msg = "abcdefghijklmnopqrstuvwxyz\0";
    char *msg2 = "zyxwvutsrqponmlkjihgfedcba\0";
    crc = _crc8(msg, strlen(msg), _URPC_CRC_POLY);
    assert_int_equal(crc, 0x90);
    crc = _crc8(msg2, strlen(msg), _URPC_CRC_POLY);
    assert_int_equal(crc, 0x96);
}


int main (int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_message_crc),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
