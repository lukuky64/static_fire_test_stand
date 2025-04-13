#include "Igniter.hpp"

Igniter::Igniter() {}

Igniter::~Igniter() {}

void Igniter::init(uint8_t controlPin, uint8_t sensePin, uint8_t armedPin) {
  m_igniterControlPin = controlPin;
  m_igniterSensePin = sensePin;
  m_igniterArmedPin = armedPin;

  pinMode(m_igniterControlPin, OUTPUT);
  digitalWrite(m_igniterControlPin, LOW);

  pinMode(m_igniterSensePin, INPUT);
  // analogSetAttenuation(ADC_11db); // 150 mV ~ 2450 mV, attenuation is 11dB by
  // default so won't need this line

  pinMode(m_igniterArmedPin, INPUT);
}

bool Igniter::igniterReady() {
  if (sytemArmed()) {
    float filteredVoltage =
        (float)analogReadMilliVolts(m_igniterSensePin) / 1000.0f;

    uint16_t numReadings = 10;

    for (int i = 1; i < numReadings; i++) {
      float newReading =
          (float)analogReadMilliVolts(m_igniterSensePin) / 1000.0f;
      filteredVoltage =
          newReading * (1 - m_alpha) + (filteredVoltage * m_alpha);
      vTaskDelay(pdMS_TO_TICKS(10));
    }

    float resistance = 18.73f * (filteredVoltage - 0.685f);

    ESP_LOGI(TAG, "Resistance reading: %f", resistance);

    if ((resistance < 6) && (resistance > 0.5)) {
      ESP_LOGI(TAG, "Igniter OK!");
      return true;
    } else {
      ESP_LOGE(TAG, "Igniter faulty!");
      return false;
    }
  } else {
    ESP_LOGE(TAG, "System not armed!");
    return false;
  }
}

bool Igniter::sytemArmed() {
  if (digitalRead(m_igniterArmedPin) == HIGH) {
    return false;
  }

  ESP_LOGI(TAG, "System armed!");
  return true;
}

bool Igniter::sendIgnition(String password) {
  if (password != m_password) {
    ESP_LOGE(TAG, "Incorrect password!");
    return false;
  }

  digitalWrite(m_igniterControlPin, HIGH);
  delay(m_igniteDuration_ms);
  // !!! need to perform testing. Based on a datasheet, 10ms at 12V is a
  // minimum. The shorter the period, the less likely we will have short causing
  // damage.
  digitalWrite(m_igniterControlPin, LOW);
  return true;
}
