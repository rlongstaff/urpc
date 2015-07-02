

#include <stdio.h>
#include <stdlib.h>

#include "urpc.h"

int main (int argc, char **argv) {
    printf("%lu\n", MAX_MESSAGE_SIZE);
    printf("%lu\n", sizeof(struct urpc_header));
    printf("%lu\n", sizeof(struct urpc_message));
    return 1;
}
