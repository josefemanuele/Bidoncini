#include "driver/rtc_io.h"
#include "esp_sleep.h"
#include "headers/sleep.h"
#include "esp_log.h"

char *TAG_SLEEP = "SLEEP_MODULE";
int sleep_delay;

/**
 * Setup deep sleep wake up sources with default values.
*/
void setup_deep_sleep(int seconds)
{
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << EXT1_WAKEUP_PIN;

    sleep_delay = seconds;
    // Enable rtc timer wakeup source.
	ESP_LOGI(TAG_SLEEP, "Enabling timer wakeup, %ds", sleep_delay);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(sleep_delay * 1000000));
    // Enable rtc pin ext0 wakeup source.
	ESP_LOGI(TAG_SLEEP, "Enabling EXT0 wakeup on pin GPIO%d", EXT0_WAKEUP_PIN);
    ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(EXT0_WAKEUP_PIN, 1));
    // Enable rtc pin ext1 wakeup source
	ESP_LOGI(TAG_SLEEP, "Enabling EXT1 wakeup on pin GPIO%d", EXT1_WAKEUP_PIN);
    ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask, ESP_EXT1_WAKEUP_ANY_HIGH));
}

/**
 * Update deep sleep timer wake up source value.
*/
void update_deep_sleep_timer(int seconds)
{
    sleep_delay = seconds;
    // Enable rtc timer wakeup source.
	ESP_LOGI(TAG_SLEEP, "Enabling timer wakeup, %ds", sleep_delay);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(sleep_delay * 1000000));
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
	        ESP_LOGI(TAG_SLEEP, "Wake up from timer.");
            cause = 0;
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT0: {
	        ESP_LOGI(TAG_SLEEP, "Wake up from ext0");
            cause = 1;
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT1: {
	        ESP_LOGI(TAG_SLEEP, "Wake up from ext1");
            cause = 2;
            break;
        }

        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
	        ESP_LOGI(TAG_SLEEP, "Not a deep sleep reset.");
            cause = -1;
    }
    return cause;
}

/**
 * Go into deep sleep.
*/
void deep_sleep(void)
{
	ESP_LOGI(TAG_SLEEP, "Going to deep sleep for %d seconds.", sleep_delay);
    esp_deep_sleep_start();
}