#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/distance.c"
#include "ra01s.h"

char *ID = "ABC123";
char *TAG_MAIN_SENSOR = "MAIN_SENSOR";


void initialize_lora(){
    uint8_t spreadingFactor = 7;
	uint8_t bandwidth = 4;
	uint8_t codingRate = 1;
	uint16_t preambleLength = 8;
	uint8_t payloadLen = 0;
	bool crcOn = true;
	bool invertIrq = false;
    LoRaConfig(spreadingFactor, bandwidth, codingRate, preambleLength, payloadLen, crcOn, invertIrq);
}


void task_sensing(void *pvParameters)
{
    int distance = measure();

    ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1261/62/68 is 255
	TickType_t nowTick = xTaskGetTickCount();
    time_t clk = time(NULL);
	int txLen = sprintf((char *)buf, "{ID: %s, distance: %d, datetime: %s}", ID, distance, ctime(&clk));
	ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent...", txLen);

	// Wait for transmission to complete
	if (LoRaSend(buf, txLen, SX126x_TXMODE_SYNC) == false) {
		ESP_LOGE(pcTaskGetName(NULL),"LoRaSend fail");
	}

	int lost = GetPacketLost();
	if (lost != 0) {
		ESP_LOGW(pcTaskGetName(NULL), "%d packets lost", lost);
	}

	//DEEP SLEEP
}


void app_main(void)
{
    setup_distance_sensor();
    
    initialize_lora();
    
    xTaskCreate(&task_sensing, "distance", 1024*4, NULL, 5, NULL);
}
