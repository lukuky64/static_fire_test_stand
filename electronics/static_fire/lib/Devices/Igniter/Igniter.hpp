#pragma once

#include "Arduino.h"
#include "esp_log.h"
#include "freeRTOS/task.h"
#include "freertos/FreeRTOS.h"

class Igniter {
 public:
  Igniter();
  ~Igniter();

  bool sendIgnition(String password);
  void init(uint8_t controlPin, uint8_t sensePin, uint8_t armedPin);
  bool igniterReady();
  bool sytemArmed();

 private:
  uint8_t m_igniterControlPin;
  uint8_t m_igniterSensePin;
  uint8_t m_igniterArmedPin;

  uint16_t m_igniteDuration_ms = 100;

  static constexpr const char *TAG = "Igniter";

  const String m_password = "12345";

  float m_alpha = 0.5;  // analog filter coefficient
};
