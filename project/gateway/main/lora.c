#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include <time.h>

#include "include/lora.h"
#include "../../../components/ra01s/ra01s.h"

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

void lora_receive_messages(uint8_t* buf, int len){
    while(1){
		u_int8_t rxLen = LoRaReceive(buf, len);
		if ( rxLen > 0 ) { //if message is received
			ESP_LOGI(TAG_LORA, "%d byte packet received:[%.*s]", rxLen, rxLen, buf);

			int8_t rssi, snr;
			GetPacketStatus(&rssi, &snr);
			ESP_LOGI(TAG_LORA, "rssi=%d[dBm] snr=%d[dB]", rssi, snr);
			return;
		}
		vTaskDelay(10); // Avoid WatchDog alerts
	}
}