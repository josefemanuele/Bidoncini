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

char *ID = "ABC123";
char *TAG_MAIN_SENSOR = "MAIN_SENSOR";
int FREQUENCY_ANALYSIS_SLEEP_S = 10;
RTC_DATA_ATTR int WAKEUP_DELAY_S;
RTC_DATA_ATTR int MEASUREMENT_NUMBER = 0;
RTC_DATA_ATTR bool need_frequency_analysis = true;
int HEIGHT_BIN_CM = 40;


/**
 * Principal task of the device. It measures the fullness of the trash bin, sends the value to the gateway via LoRa,
 * and goes into deep sleep
*/
void task_sensing()
{
    //measure the distance
    int distance = measure();
    //send the info via LoRa
    lora_message_send(ID, distance);
}

void deep_sleep(void)
{
    // Set deep sleep timer.
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(WAKEUP_DELAY_S * 1000000));
    ESP_LOGI(TAG_MAIN_SENSOR, "Going to deep sleep for %d seconds.", WAKEUP_DELAY_S);
    // Enter deep sleep.
    esp_deep_sleep_start();
}

void frequency_analysis(){
    while (1){
        int distance = measure();
        MEASUREMENT_NUMBER++;
        if ((distance/HEIGHT_BIN_CM)*100 <= 10){
            WAKEUP_DELAY_S = (MEASUREMENT_NUMBER * WAKEUP_DELAY_S) / 4;
            break;
        }
        else {
            deep_sleep();
        }  
    }
}

void app_main(void)
{
    while (1)
    {
        setup_distance_sensor();
        
        initialize_lora();

        if(need_frequency_analysis){
            WAKEUP_DELAY_S = FREQUENCY_ANALYSIS_SLEEP_S;
            frequency_analysis();
        }
        
        task_sensing();

        deep_sleep();
    }
}
