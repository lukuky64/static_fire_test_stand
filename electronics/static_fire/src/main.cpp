#include <Arduino.h>

#include "State_Machine.hpp"
#include "esp_log.h"

// #include "Devices.hpp"

// Devices devices;

State_Machine stateMachine;

void setup() {
  stateMachine.begin();  // Start the state machine

  // delay(100);
  // devices.begin();

  // Serial.println("Setup complete!");
}

void loop() { vTaskDelay(portMAX_DELAY); }
