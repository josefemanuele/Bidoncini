#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "wolfssl/wolfcrypt/types.h"

#include "include/wifi.h"
#include "include/mqtt.h"
#include "include/lora.h"
#include "../../components/ra01s/ra01s.h"

#define LORA_FREQUENCY 868300000
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
    initialize_lora();
    //setup ECC keys
    import_keys();
}

/*
  task that listens for LoRa packets and publish the packet on MQTT
*/
void task_receiver_lora(void *pvParameters)
{
	ESP_LOGI(TAG_MAIN, "Start");
	u_int8_t buf[256]; // Maximum Payload size of SX1261/62/68 is 255
	while(1) {
    //wait to receive a message
    lora_receive_messages(buf, sizeof(buf));

    //decrypt the message
    byte decrypted[256];
    word32 decryptedSz = (word32) sizeof(decrypted);
    decrypt_message(buf, decrypted, decryptedSz);

    //publish on MQTT
    mqtt_publish((char *)buf);

    vTaskDelay(10); // Avoid WatchDog alerts
	}
		
}


void app_main(void)
{
    ESP_LOGI(TAG_MAIN, "INIT");

    initialization();

    xTaskCreate(&task_receiver_lora, "RX", 1024*4, NULL, 5, NULL);
}
