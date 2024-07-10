#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include <time.h>
#include "headers/lora.h"
#include "headers/crypto.h"
#include "../../../components/ra01s/ra01s.h"
#include "wolfssl/wolfcrypt/types.h"

char *TAG_LORA = "LORA_MODULE";

void initialize_lora(){
    uint8_t spreadingFactor = 7;
	uint8_t bandwidth = 4;
	uint8_t codingRate = 1;
	uint16_t preambleLength = 8;
	uint8_t payloadLen = 0;
	bool crcOn = true;
	bool invertIrq = false;
	LoRaInit();
	LoRaBegin(LORA_FREQUENCY, 22, 3.3, true);
    LoRaConfig(spreadingFactor, bandwidth, codingRate, preambleLength, payloadLen, crcOn, invertIrq);
}

void lora_message_send(char* ID, int distance){
    uint8_t buf[256]; // Maximum Payload size of SX1261/62/68 is 255
	char distance_in_char[4];

    //build the message
	unsigned char encrypted[256];
    word32 encryptedSz = sizeof(encrypted);
	sprintf(distance_in_char, "%d", distance);

	int txLen = sprintf((char *)buf, "{\"id\": \"%s\", \"distance\": %d}", 
							ID, distance);
	LoRaSend(buf, txLen, SX126x_TXMODE_SYNC);
	encrypt_value((char*)buf, (word32)strlen((char*)buf), encrypted, &encryptedSz);
		
	ESP_LOGI(TAG_LORA, "%d byte packet sent...", txLen);

	// Wait for transmission to complete
	if (LoRaSend(encrypted, encryptedSz, SX126x_TXMODE_SYNC) == false) {
		ESP_LOGE(TAG_LORA,"LoRaSend fail");
	}
	ESP_LOGI(TAG_LORA, "LoRa packet sent");

    //check for packets lost
	int lost = GetPacketLost();
	if (lost != 0) {
		ESP_LOGW(TAG_LORA, "%d packets lost", lost);
	}
}