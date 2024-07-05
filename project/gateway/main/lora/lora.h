#define LORA_FREQUENCY 868300000

/**
 * @brief Initializes the LoRa module with the specified parameters.
 */
void initialize_lora(void);

/**
 * @brief Waits for a message over LoRa, and when a message is received, it saves it into buf.
 * 
 * @param buf A pointer to a uint8_t array that will contain the content of the LoRa message.
 */
void lora_receive_messages(uint8_t *buf);