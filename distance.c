#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include <esp_timer.h>
#include <inttypes.h>

#define PING_TIMEOUT 6000
#define SPEED_OF_SOUND_AT_22C_CM_US 0.0343 // Speed of sound in cm/us at 0 degrees Celsius
#define TRIG_PIN GPIO_NUM_8
#define ECHO_PIN GPIO_NUM_9

#define timeout_expired(start, len) ((esp_timer_get_time() - (start)) >= (len))

long duration;
float distanceCm;

void setup() {
  gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
}

void loop() {
  gpio_set_level(TRIG_PIN, LOW);
  ets_delay_us(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  gpio_set_level(TRIG_PIN, HIGH);
  ets_delay_us(10);
  gpio_set_level(TRIG_PIN, LOW);
  if (gpio_get_level(ECHO_PIN)){
    printf("EEROR");
    return;
  }


    // Wait for echo
  int64_t start = esp_timer_get_time();
  while (!gpio_get_level(ECHO_PIN)){
    if(timeout_expired(start, PING_TIMEOUT)){
      printf("ERROR TIMEOUT");
      return;
    }
  }

  // got echo, measuring
  int64_t echo_start = esp_timer_get_time();
  int64_t time = echo_start;
  while (gpio_get_level(ECHO_PIN)){
    time = esp_timer_get_time();
  }

  int64_t time_us = time - echo_start;
  printf("time_us = %d\n", time_us);

  int distance = (time_us) * SPEED_OF_SOUND_AT_22C_CM_US /2;

  
  // Prints the distance in the Serial Monitor
  printf("Distance (cm): ");
  printf("%d\n", distance);
  
  vTaskDelay(pdMS_TO_TICKS(1000));
}
