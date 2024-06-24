#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "wifi/wifi.h"
#include "mqtt/mqtt.h"
#include "ra01s.h"

char *TAG_MAIN = "MAIN";

/**
 * Initialize all the needed channel of communication. WiFi, MQTT and ADC.
*/
void  initialization(){

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //initialize WiFi
    wifi_init_sta();
    vTaskDelay(10000/ portTICK_PERIOD_MS);
    ESP_LOGI(TAG_MAIN, "WiFi initialized");
    //initialize MQTT
    mqtt_app_start();
    ESP_LOGI(TAG_MAIN, "MQTT initialized");
    //initialize LoRa
    uint8_t spreadingFactor = 7;
	  uint8_t bandwidth = 4;
	  uint8_t codingRate = 1;
	  uint16_t preambleLength = 8;
	  uint8_t payloadLen = 0;
	  bool crcOn = true;
	  bool invertIrq = false;
    LoRaConfig(spreadingFactor, bandwidth, codingRate, preambleLength, payloadLen, crcOn, invertIrq);
}

/*
  task that listens for LoRa packets and publish the packet on MQTT
*/
void task_receiver_lora(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1261/62/68 is 255
	while(1) {
		uint8_t rxLen = LoRaReceive(buf, sizeof(buf));
		if ( rxLen > 0 ) { 
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", rxLen, rxLen, buf);

			int8_t rssi, snr;
			GetPacketStatus(&rssi, &snr);
			ESP_LOGI(pcTaskGetName(NULL), "rssi=%d[dBm] snr=%d[dB]", rssi, snr);
      //publish on MQTT
      mqtt_publish(*buf);
		}
		vTaskDelay(1); // Avoid WatchDog alerts
	}
}


void app_main(void)
{
    ESP_LOGI(TAG_MAIN, "INIT");

    initialization();

    xTaskCreate(&task_receiver_lora, "RX", 1024*4, NULL, 5, NULL);
}
