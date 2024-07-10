#include "freertos/FreeRTOS.h"
#include "wolfssl/wolfcrypt/types.h"

/**
 * @brief Importa le chiavi ECC pubblica e privata.
 *
 * Questa funzione inizializza le chiavi ECC e le importa da array di byte
 * predefiniti. Inizializza anche il contesto ECC per la decrittografia.
 */
void import_keys();

/**
 * @brief Decripta un messaggio crittografato utilizzando ECC.
 *
 * @param[in] buf Il buffer contenente il messaggio crittografato.
 * @param[out] out Il buffer dove sarà memorizzato il messaggio decrittografato.
 * @param[in,out] outSz Un puntatore alla dimensione del buffer di output. Dopo la chiamata, contiene la dimensione effettiva del messaggio decrittografato.
 */
void decrypt_message(u_int8_t* buf, byte* out, word32* outSz);