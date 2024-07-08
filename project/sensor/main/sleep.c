#include "driver/rtc_io.h"
#include "esp_sleep.h"
#include "headers/sleep.h"

/**
 * Setup deep sleep wake up sources with default values.
*/
void setup_deep_sleep(void)
{
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << EXT1_WAKEUP_PIN;

    // Enable rtc timer wakeup source.
    printf("Enabling timer wakeup, %ds\n", DEFAULT_DEEP_SLEEP_DELAY);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(DEFAULT_DEEP_SLEEP_DELAY * 1000000));
    // Enable rtc pin ext0 wakeup source.
    printf("Enabling EXT0 wakeup on pin GPIO%d\n", EXT0_WAKEUP_PIN);
    ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(EXT0_WAKEUP_PIN, 1));
    // Enable rtc pin ext1 wakeup source
    printf("Enabling EXT1 wakeup on pin GPIO%d\n", EXT1_WAKEUP_PIN);
    ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup_io(ext_wakeup_pin_1_mask, ESP_EXT1_WAKEUP_ANY_HIGH));
}

/**
 * Update deep sleep timer wake up source value.
*/
void update_deep_sleep_timer(int seconds)
{
    // Enable rtc timer wakeup source.
    printf("Enabling timer wakeup, %ds\n", seconds);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(seconds * 1000000));
}

/**
 *  Queries wake up cause.
 *  Return:
 *  0 for time up.
 *  1 for ext0 rtc wakeup.
 *  2 for ext1 rtc wakeup.
 *  -1 for undefined.
 */

int get_wakeup_cause(void)
{
    int cause = -1;
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer.\n");
            cause = 0;
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT0: {
            printf("Wake up from ext0\n");
            cause = 1;
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT1: {
            printf("Wake up from ext1\n");
            cause = 2;
            break;
        }

        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
            cause = -1;
    }
    return cause;
}

/**
 * Go into deep sleep.
*/
void deep_sleep(void)
{
    esp_deep_sleep_start();
}