#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
/* WOLFSSL includes. */
#define WOLFSSL_ESPIDF
#define WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/curve25519.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include "headers/crypto.h"

int LEN = 128;      //length of ECC key
RTC_DATA_ATTR byte my_key_public[LEN];
RTC_DATA_ATTR byte my_key_private[LEN];
RTC_DATA_ATTR ecc_key my_key;
RTC_DATA_ATTR ecc_key server_pub[LEN];

byte server_public_key_bytes[LEN] = "0xababab";
char *TAG_CRYPTO = "CRYPTO_MODULE";

void print_bytes(const uint8_t *bytes, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i != 0 && i != len - 1 && i % BYTES_PER_LINE == 0)
        {
            printf("\n");
        }

        printf("0x%02X\t", *(bytes + i));
    }
    printf("\n");
}

void generate_keys(){
    ecc_key key;
    WC_RNG rng;

    //initialize ECC key and RNG
    ESP_LOGI(TAG_CRYPTO, "Initializing ECC key and RNG");
    wc_ecc_init(&key);
    wc_InitRng(&rng);
    //generatig ECC key
    ESP_LOGI(TAG_CRYPTO, "Generating ECC key");
    wc_ecc_make_key(&rng, 32, &key);
    my_key = key;
    //export ECC key
    ESP_LOGI(TAG_CRYPTO, "Exporting public and private key");
    wc_ecc_export_x963(&key, my_key_public, &LEN);
    wc_ecc_export_private_only(&key, my_key_private, &LEN);

    ESP_LOGI(TAG_CRYPTO, "Printing public key");    
    print_bytes(my_key_public, LEN);
    printf("\n");

    ecc_key key2;
    wc_ecc_init(&key2);
    byte server_key_public[LEN];
    byte server_key_private[LEN]; 
    //generatig ECC key
    wc_ecc_make_key(&rng, 32, &key2);
    //export ECC key
    wc_ecc_export_x963(&key2, server_key_public, &LEN);
    wc_ecc_export_private_only(&key2, server_key_private, &LEN);

    print_bytes(server_key_public, LEN);
    printf("\n");

    print_bytes(server_key_private, LEN);
    printf("\n");


    //import server public key
    ESP_LOGI(TAG_CRYPTO, "Importing the server public key");    
    wc_ecc_init(&server_pub);
    wc_ecc_import_x963(server_public_key_bytes, sizeof(server_public_key_bytes), &server_pub);
}

void encrypt_value(char* message, unsigned char* encrypted, word32 encryptedSz){
    const unsigned char* msg; 
    word32 msgLen;
    msg = (unsigned char*)message;
    msgLen = (word32)strlen((const char*)msg);
    encryptedSz = sizeof(encrypted);
    //wc_ecc_encrypt(&my_key, &server_pub, msg, msgLen, encrypted, &encryptedSz);
}
