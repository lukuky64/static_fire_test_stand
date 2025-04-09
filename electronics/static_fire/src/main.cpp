#include <Arduino.h>

#include "State_Machine.hpp"
// #include "esp_log.h"

#include "Devices.hpp"

// Devices devices;

State_Machine stateMachine;

// #include "Commander.hpp"

// Commander commander(115200);  // Create a Commander instance

void setup() {
  // USBSerial.begin(115200);  // Initialize USBSerial with a baud rate of
  // 115200
  // commander.init();  // Initialize the commander
  stateMachine.begin();  // Start the state machine

  // delay(100);
  // devices.begin();

  // Serial.println("Setup complete!");
}

void loop() {
  // commander.run();  // Run the command parser
  vTaskDelay(portMAX_DELAY);
}
