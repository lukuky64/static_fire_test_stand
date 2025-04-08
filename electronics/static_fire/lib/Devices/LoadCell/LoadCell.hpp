#pragma once

#include "Arduino.h"
#include "MCP4561_DIGI_POT.hpp"
#include "esp_log.h"

class LoadCell {
 public:
  LoadCell();
  ~LoadCell();

  void init(TwoWire &I2C_Bus, uint8_t sensorPin);
  bool isReady();
  bool calibrate();
  float getSample_mV();
  float getAveragedSamples(uint16_t numReadings);
  bool trimRef(float mV_offset);

 private:
  bool setTrimPos(uint16_t newPos);

  MCP4561 m_digitalTrim;  // For trimming output offset
  uint8_t m_sensorPin;

  float m_alpha;

  bool m_calibrated = false;

  float m_currentOffset_mV;
  uint16_t m_currentPotPos = 0;
  const float m_potVoltageRange = 3300.0f;
  const float m_potTaps = 129.0f;  // number of taps

  static constexpr const char *TAG = "LoadCell";
};
