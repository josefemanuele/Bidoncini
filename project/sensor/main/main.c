#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include <time.h>
#include "sys/time.h"
#include "driver/distance.c"
#include "lora/lora.c"
#include "esp_sleep.h"

char *ID = "ABC123";        //ID of the bin
char *TAG_MAIN_SENSOR = "MAIN_SENSOR";
RTC_DATA_ATTR int WAKEUP_DELAY_S = 10;       //time between measurements
RTC_DATA_ATTR int MEASUREMENT_NUMBER = 0;       //number of measurement since last time has been emptied
int EXPECTED_MEASUREMENTS = 4;        //expected number of measurement to do until trash bin is full
int HEIGHT_BIN_CM = 40;

#define i_am_full(distance) (((distance/HEIGHT_BIN_CM)*100) <= (10)) //checks if the bin is full (at 90%)


/**
 * Principal task of the device. It measures the fullness of the trash bin, sends the value to the gateway via LoRa,
 * and goes into deep sleep
*/
void task_sensing()
{
    //measure the distance
    int distance = measure();
    MEASUREMENT_NUMBER ++;
    //send the info via LoRa
    lora_message_send(ID, distance);
    //check if the bin has fullen up faster (or later) than what expected and adjust sleep time
    if (i_am_full(distance) && ((MEASUREMENT_NUMBER <= EXPECTED_MEASUREMENTS / 2) || (MEASUREMENT_NUMBER >= EXPECTED_MEASUREMENTS * 2))){
        WAKEUP_DELAY_S = (MEASUREMENT_NUMBER * WAKEUP_DELAY_S) / EXPECTED_MEASUREMENTS;
    }

}

/**
 * performs the deep sleep for a number of seconds indicated by WAKEUP_DELAY_S
*/
void deep_sleep(void)
{
    // Set deep sleep timer.
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(WAKEUP_DELAY_S * 1000000));
    ESP_LOGI(TAG_MAIN_SENSOR, "Going to deep sleep for %d seconds.", WAKEUP_DELAY_S);
    // Enter deep sleep.
    esp_deep_sleep_start();
}

void app_main(void)
{
    while (1)
    {
        setup_distance_sensor();
        
        initialize_lora();
        
        task_sensing();

        deep_sleep();
    }
}
