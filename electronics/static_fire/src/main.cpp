#include <Arduino.h>

#include "State_Machine.hpp"
// #include "esp_log.h"

#include "Devices.hpp"

// Devices devices;

State_Machine stateMachine;

// #include "Commander.hpp"

// Commander commander(115200);  // Create a Commander instance

void setup() {
  stateMachine.begin();  // Start the state machine
  // pinMode(GPIO_NUM_6, INPUT);
}

void loop() {
  // commander.run();  // Run the command parser

  // float newReading = (float)analogReadMilliVolts(GPIO_NUM_6) / 1000.0f;
  // ESP_LOGI("TAG", "Raw reading: %f", newReading);

  // uint16_t newReading = analogRead(GPIO_NUM_6);
  // ESP_LOGI("TAG", "Raw reading: %u", newReading);
  // vTaskDelay(250);

  vTaskDelay(portMAX_DELAY);
}
