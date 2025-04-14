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
  float getAveragedSamplesMv(uint16_t numReadings);
  bool trimRef(float mV_offset);
  float getForceSample();

 private:
  bool setTrimPos(uint16_t newPos);

  MCP4561 m_digitalTrim;  // For trimming output offset
  uint8_t m_sensorPin;

  float m_alpha;

  bool m_calibrated = false;

  float m_currentOffset_mV = 0.0f;
  uint16_t m_currentPotPos = 0;
  const float m_potVoltageRange = 3300.0f;
  const float m_potTaps = 129.0f;  // number of taps

  const float mid_mV = 3300.0f / 2.0f;  // midscale

  static constexpr const char *TAG = "LoadCell";
};
