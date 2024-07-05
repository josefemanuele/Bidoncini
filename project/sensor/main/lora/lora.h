#define LORA_FREQUENCY 868300000

/**
 * @brief Initializes the LoRa module with the specified parameters.
 */
void initialize_lora(void);

/**
 * @brief Sends a message over LoRa with the given ID and distance.
 * 
 * @param ID A pointer to a character array representing the ID of the device.
 * @param distance An integer representing the fullness of the trash bin.
 */
void lora_message_send(char* ID, int distance);