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
int WAKEUP_DELAY_S = 3;


void set_time_manual(int year, int month, int day, int hour, int minute, int second)
{
    struct tm tm;
    time_t t;

    // Time information
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    // Convert struct tm in time_t
    t = mktime(&tm);

    // Set up the sys time
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}


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
    ESP_LOGI(TAG, "Going to deep sleep for %d seconds.", WAKEUP_DELAY_S);
    // Enter deep sleep.
    esp_deep_sleep_start();
}

void app_main(void)
{
    while (1)
    {
        setup_distance_sensor();
        
        initialize_lora();

        set_time_manual(2024, 7, 5, 11, 30, 0);
        
        task_sensing();

        deep_sleep();
    }
}
