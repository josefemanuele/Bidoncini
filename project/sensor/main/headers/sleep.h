#define DEFAULT_DEEP_SLEEP_DELAY 5
#define EXT0_WAKEUP_PIN  GPIO_NUM_1
#define EXT1_WAKEUP_PIN  GPIO_NUM_2

/**
 * Setup deep sleep wake up sources with default values.
*/
void setup_deep_sleep(int seconds);

/**
 * Update deep sleep timer wake up source value.
*/
void update_deep_sleep_timer(int seconds);

/**
 *  Queries wake up cause.
 *  Return:
 *  0 for time up.
 *  1 for ext0 rtc wakeup.
 *  2 for ext1 rtc wakeup.
 *  -1 for undefined.
 */

int get_wakeup_cause(void);

/**
 * Go into deep sleep.
*/
void deep_sleep(void);