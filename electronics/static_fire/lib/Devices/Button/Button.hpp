#pragma once

#include "Arduino.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Button {
 public:
  Button(uint8_t pin, uint32_t debounceDelay = 50);
  ~Button();

  bool isPressed();
  bool isReleased();
  bool isLongPress();
  bool isDoublePress();

 private:
  uint8_t m_pin;
  uint32_t m_debounceDelay;
  uint32_t m_lastDebounceTime;
  bool m_lastButtonState;
  bool m_buttonState;
};