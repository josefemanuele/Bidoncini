#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/distance.c"

void app_main(void)
{
    setup();
    while(1){
        measure();
    }
}
