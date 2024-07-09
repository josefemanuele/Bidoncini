#include "wolfssl/wolfcrypt/types.h"

void print_bytes(const uint8_t *bytes, int len);

void generate_keys(void);

void encrypt_value(char* message, word32 msgLen, unsigned char* encrypted, word32 encryptedSz);