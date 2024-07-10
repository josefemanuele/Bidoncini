#include "wolfssl/wolfcrypt/types.h"

/**
 * @brief Prints the bytes in hexadecimal.
 *
 * @param[in] bytes Reference to the bytes to print.
 * @param[in] len Length of bytes array.
 */
void print_bytes(const uint8_t *bytes, int len);

/**
 * @brief Generation of the ECC key and import of the server key.
 *
 * This function generates the ECC key, export it in format x593.
 */
void generate_keys();

/**
 * @brief Encrypt a message using ECC.
 *
 * @param[in] message Message to encrypt.
 * @param[in] msgLen Length of the message.
 * @param[out] encrypted Buffer that will contain the encrypted message.
 * @param[in,out] encryptedSz Will contain the actual length od=f the encrypted message.
 */
void encrypt_value(char* message, word32 msgLen, unsigned char* encrypted, word32* encryptedSz);