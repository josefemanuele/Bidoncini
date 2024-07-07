#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include <esp_timer.h>
#include <inttypes.h>
#include "esp_log.h"
#include <esp_err.h>

#include "distance.h"

char *TAG_DISTANCE = "DISTANCE SENSOR";

long duration;
float distanceCm;


void setup_distance_sensor() {
  gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
}



int measure() {
  gpio_set_level(TRIG_PIN, LOW);
  esp_rom_delay_us(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  gpio_set_level(TRIG_PIN, HIGH);
  esp_rom_delay_us(10);
  gpio_set_level(TRIG_PIN, LOW);
  if (gpio_get_level(ECHO_PIN)){
    ESP_LOGI(TAG_DISTANCE, "ERROR, echo_pin remained high");
    return 0;
  }

  // Wait for echo
  int64_t start = esp_timer_get_time();
  while (!gpio_get_level(ECHO_PIN)){
    if(timeout_expired(start, PING_TIMEOUT)){
      ESP_LOGI(TAG_DISTANCE, "ERROR TIMEOUT expired");
      return 0;
    }
  }

  // got echo, measuring
  int64_t echo_start = esp_timer_get_time();
  int64_t time = echo_start;
  while (gpio_get_level(ECHO_PIN)){
    time = esp_timer_get_time();
  }

  int64_t time_us = time - echo_start;
  ESP_LOGI(TAG_DISTANCE, "time_us = %lld", time_us);

  int distance = (time_us) * SPEED_OF_SOUND_AT_22C_CM_US /2;

  
  // Prints the distance in the Serial Monitor
  ESP_LOGI(TAG_DISTANCE, "Distance (cm): %d\n", distance);
  
  return distance;
}
