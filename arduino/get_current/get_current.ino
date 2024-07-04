#include <Wire.h>
#include <Adafruit_INA219.h>

// Loop delay in ms.
unsigned long LOOP_DELAY = 1;
Adafruit_INA219 ina219;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // will pause until serial console opens
    delay(1);
  }

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A). However
  // you can call a setCalibration function to change this range (see comments). 
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps): 
  ina219.setCalibration_16V_400mA();
}
void loop(void) {
  float current_mA = 0;

  current_mA = ina219.getCurrent_mA();
  Serial.println(current_mA); 
  delay(LOOP_DELAY);
}