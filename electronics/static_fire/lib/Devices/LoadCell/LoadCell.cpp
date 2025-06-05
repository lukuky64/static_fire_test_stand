#include "LoadCell.hpp"

#include <algorithm>

LoadCell::LoadCell() {}

void LoadCell::init(TwoWire &I2C_Bus, uint8_t sensorPin) {
  m_digitalTrim.init(0x2E, I2C_Bus);

  if (m_digitalTrim.potConnectAll(MCP4561_WIPER_0) == MCP4561_SUCCESS) {
    ESP_LOGI(TAG, "Pot connected!");
  } else {
    ESP_LOGI(TAG, "Pot NOT connected!");
  }

  m_sensorPin = sensorPin;
  pinMode(m_sensorPin, INPUT);

  uint16_t value = 64;  // midscale for initialisation
  setTrimPos(value);
}

LoadCell::~LoadCell() {}

bool LoadCell::isReady() { return m_calibrated; }

bool LoadCell::calibrate() {
  m_currentOffset_mV = getAveragedSamplesMv(20) - mid_mV;
  ESP_LOGI(TAG, "Load cell offset before calibration: %f mV",
           m_currentOffset_mV);

  if (trimRef(m_currentOffset_mV)) {
    m_currentOffset_mV = getAveragedSamplesMv(20) - mid_mV;
    ESP_LOGI(TAG, "Load cell offset after hardware calibration: %f mV",
             m_currentOffset_mV);

    // mid_mV += m_currentOffset_mV;  // additional software offset

    // m_currentOffset_mV = getAveragedSamplesMv(20) - mid_mV;
    // ESP_LOGI(TAG, "Load cell offset after software calibration: %f mV",
    //          m_currentOffset_mV);

    m_calibrated = true;
    return true;
  }
  m_currentOffset_mV = 0.0f;  // reset offset if calibration fails
  return false;
}

float LoadCell::getAveragedSamplesMv(uint16_t numReadings) {
  float offsetmV = static_cast<float>(getSample_mV());

  for (int i = 1; i < numReadings; i++) {
    float newSample = static_cast<float>(getSample_mV());
    offsetmV =
        newSample * (1 - m_alpha) + (offsetmV * m_alpha);  // simple filter
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  return offsetmV;
}

float LoadCell::getSample_mV() {
  return (float)analogReadMilliVolts(m_sensorPin);  // !!! test this
}

float LoadCell::getForceSample() {
  float value = getSample_mV() - m_currentOffset_mV - mid_mV;
  value = value * 9.04;  // 4.464N/mV
  return value;
  // !! will need some multiplier here
}

bool LoadCell::trimRef(float mV_offset) {
  // other method is to use increment/decrement. this will be slower BUT we can
  // implement feedback.

  // Max voltage=3.3, min=0, step=25.58mV
  static const float stepSize = m_potVoltageRange / m_potTaps;

  // how many steps to get to value from current pos
  int16_t signedSteps = static_cast<int16_t>(round(mV_offset / stepSize));

  // value may be negative due to rounding and accuracy
  uint16_t newPosSigned = m_currentPotPos + signedSteps;

  // ensure newPos is within the bounds
  uint16_t newPosUnSigned =
      std::max(static_cast<uint16_t>(0),
               std::min(newPosSigned, static_cast<uint16_t>(m_potTaps)));

  return setTrimPos(newPosUnSigned);
}

bool LoadCell::setTrimPos(uint16_t newPos) {
  if (m_digitalTrim.writeVal(MCP4561_VOL_WIPER_0, newPos) == MCP4561_SUCCESS) {
    m_currentPotPos = newPos;
    ESP_LOGI(TAG, "Set trim pos %d", newPos);
    return true;
  } else {
    ESP_LOGI(TAG, "Cannot set trim pos %d", newPos);
    return false;
  }
}