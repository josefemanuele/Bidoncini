
#define PING_TIMEOUT 6000 // Maximum timeout for ping (in microseconds)
#define SPEED_OF_SOUND_AT_22C_CM_US 0.0343 // Speed of sound in cm/us at 22 degrees Celsius
// Logical states LOW and HIGH
#define LOW 0
#define HIGH 1
// GPIO pins for the trigger and echo of the distance sensor
#define TRIG_PIN GPIO_NUM_4
#define ECHO_PIN GPIO_NUM_3

// Check if the timeout has expired
#define timeout_expired(start, len) ((esp_timer_get_time() - (start)) >= (len))

/**
 * @brief Configures the pins for the distance sensor.
 */
void setup_distance_sensor();


/**
 * @brief Measures the distance using the ultrasonic sensor.
 *
 * @return int The measured distance in centimeters.
 */
int measure();

