#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char CRC8(const char *data, unsigned char len) {
  unsigned char crc = 0x00;
  while (len--) {
    unsigned char extract = *data++;
    for (unsigned char tempI = 8; tempI; tempI--) {
      unsigned char sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

int main (int argc, char** argv) {
    char* data = "hello world\n";
    unsigned char length = strlen(data);
    printf("data: %s", data);
    printf("sum: %u\n", CRC8(data, length));
    exit(0);
}

