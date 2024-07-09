#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <esp_err.h>
#include <time.h>
#include "sys/time.h"
#include "headers/distance.h"
#include "headers/lora.h"
#include "headers/sleep.h"

char *ID = "ABC123";        //ID of the bin
char *TAG_MAIN_SENSOR = "MAIN_SENSOR";
RTC_DATA_ATTR int deep_sleep_timer_s = 10;       //time between measurements
RTC_DATA_ATTR int measurement_number = 0;       //number of measurement since last time has been emptied
int EXPECTED_MEASUREMENTS = 4;        //expected number of measurement to do until trash bin is full
int HEIGHT_BIN_CM = 40;

#define i_am_full(distance) (((float)((float)distance/(float)HEIGHT_BIN_CM)*100) <= (10)) //checks if the bin is full (at 90%)


/**
 * Principal task of the device. It measures the fullness of the trash bin, sends the value to the gateway via LoRa,
 * and goes into deep sleep
*/
void task_sensing()
{
    //check if the operator has emptied the bin
    int cause = get_wakeup_cause();
    if(cause == 2){
        measurement_number = 0;
        return;
    }
    //measure the distance
    int distance = measure();
    //int distance = 10;
    measurement_number ++;
    //send the info via LoRa
    lora_message_send(ID, distance);
    //check if the bin has fullen up it adjusts the sleep time
    printf("%f", (float)((float)distance/(float)HEIGHT_BIN_CM)*100);
    if (i_am_full(distance)){
        deep_sleep_timer_s = (measurement_number * deep_sleep_timer_s) / EXPECTED_MEASUREMENTS;
        update_deep_sleep_timer(deep_sleep_timer_s);
    }
}

void app_main(void)
{
    setup_deep_sleep(deep_sleep_timer_s);

    setup_distance_sensor();

    //generate_keys();
    
    initialize_lora();
    
    task_sensing();

    deep_sleep();
}
