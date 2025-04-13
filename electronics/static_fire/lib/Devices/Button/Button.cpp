#include "Button.hpp"

Button::Button(uint8_t pin, uint32_t debounceDelay)
    : m_pin(pin), m_debounceDelay(debounceDelay), m_lastButtonState(false) {
  pinMode(m_pin, INPUT);
  m_lastDebounceTime = millis();       // Initialize the last debounce time
  m_buttonState = digitalRead(m_pin);  // Read the initial button state
}

Button::~Button() {}

bool Button::isPressed() {
  bool currentState = digitalRead(m_pin);
  if (currentState != m_lastButtonState) {
    m_lastDebounceTime = millis();  // Reset the debounce timer
  }

  if ((millis() - m_lastDebounceTime) > m_debounceDelay) {
    if (currentState != m_buttonState) {
      m_buttonState = currentState;
      return !m_buttonState;  // Return true if button is pressed (LOW)
    }
  }

  m_lastButtonState = currentState;  // Update the last button state
  return false;
}

bool Button::isReleased() {
  return !isPressed();  // Return true if button is released (HIGH)
}

bool Button::isLongPress() {
  if (isPressed()) {
    if ((millis() - m_lastDebounceTime) > 1000) {  // 1 second long press
      return true;
    }
  }
  return false;
}

bool Button::isDoublePress() {
  static uint32_t lastPressTime = 0;
  static uint8_t pressCount = 0;

  if (isPressed()) {
    if ((millis() - lastPressTime) < 500) {  // 500 ms for double press
      pressCount++;
      if (pressCount == 2) {
        pressCount = 0;  // Reset count after double press
        return true;
      }
    } else {
      pressCount = 1;  // Reset count for single press
    }
    lastPressTime = millis();  // Update the last press time
  }

  return false;
}
