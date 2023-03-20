#include "blake3.h"
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  // Read input bytes from stdin.
  /* unsigned char buf[65536]; */
  /* while (1) { */
  /*   ssize_t n = read(STDIN_FILENO, buf, sizeof(buf)); */
  /*   if (n > 0) { */
  /*     blake3_hasher_update(&hasher, buf, n); */
  /*   } else if (n == 0) { */
  /*     break; // end of file */
  /*   } else { */
  /*     fprintf(stderr, "read failed: %s\n", strerror(errno)); */
  /*     exit(1); */
  /*   } */
  /* } */
#define PROCESS_LEN 1
  uint8_t *out = (uint8_t *)malloc(32 * PROCESS_LEN);
  uint8_t *out_process = out;
  for (auto j = 0; j < PROCESS_LEN; j++) {
    // Initialize the hasher.
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    uint8_t buf[180];
    for (auto i = 8; i < 180; i+=4) {
      buf[i] = 0xe7;
      buf[i+1] = 0xf3;
      buf[i+2] = 0x1b;
      buf[i+3] = 0x08;
    }
    uint64_t x = htobe64(0x0102030405060708 + j);
    uint8_t *ptr = (uint8_t *)&x;
    for (int i = 0; i < 8; i++) {
      buf[i] = ptr[i];
    }
    blake3_hasher_update(&hasher, buf, 180);

    // Finalize the hash. BLAKE3_OUT_LEN is the default output length, 32 bytes.
    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    // Print the hash as hexadecimal.
    for (size_t i = 0; i < BLAKE3_OUT_LEN; i++) {
      out_process[i] = output[i];
    }
    out_process += 32;
  }

  for (auto i = 0; i < PROCESS_LEN; i++) {
    for (size_t i = 0; i < BLAKE3_OUT_LEN; i++) {
      printf("%02x", out[i]);
    }
    printf("\n");
    out += 32;
  }
  return 0;
}
