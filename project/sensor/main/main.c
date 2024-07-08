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
int FREQUENCY_ANALYSIS_SLEEP_S = 10;    //time between measurements in the frequency analysis
RTC_DATA_ATTR int WAKEUP_DELAY_S;       //time between measurements
RTC_DATA_ATTR int MEASUREMENT_NUMBER = 0;       //number of measurement since last time has been emptied
int EXPECTED_MEASUREMENTS = 4;        //expected number of measurement to do until trash bin is full
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
    MEASUREMENT_NUMBER ++;
    //send the info via LoRa
    lora_message_send(ID, distance);
    //check if the bin has fullen up faster than what expected
    if (MEASUREMENT_NUMBER <= EXPECTED_MEASUREMENTS / 2){
        need_frequency_analysis = true;
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

/**
 * Performs a "frequency analysis". It checks the fullness of the bin every  30 minutes and sees how many time it takes
 * for the trash bin to be 90% full. It updates the frequency to which take the measurements accordingly
*/
void frequency_analysis(){
    while (1){
        int distance = measure();
        MEASUREMENT_NUMBER++;
        lora_message_send(ID, distance);
        //check if the bin is at least 90% full
        if ((distance/HEIGHT_BIN_CM)*100 <= 10){
            WAKEUP_DELAY_S = (MEASUREMENT_NUMBER * WAKEUP_DELAY_S) / EXPECTED_MEASUREMENTS;     //update the time interval between measurements
            need_frequency_analysis = false;
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
            WAKEUP_DELAY_S = FREQUENCY_ANALYSIS_SLEEP_S;    //set the time between measurement to the value for the  frequency analysis
            frequency_analysis();
        }
        
        task_sensing();

        deep_sleep();
    }
}
