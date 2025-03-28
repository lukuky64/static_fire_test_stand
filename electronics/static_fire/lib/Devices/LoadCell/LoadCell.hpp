#pragma once

#include "Arduino.h"
#include "esp_log.h"
#include "MCP4561_DIGI_POT.hpp"

class LoadCell
{
public:
    LoadCell();
    ~LoadCell();

    void init(TwoWire &I2C_Bus, uint8_t sensorPin);
    bool isReady();
    bool calibrate();
    float getSample_mV();
    bool trimRef(float mV_offset);

private:
    static constexpr const char *TAG = "LoadCell";
    MCP4561 m_digitalTrim; // For trimming output offset
    uint8_t m_sensorPin;

    float m_alpha;

    uint16_t m_currentPotPos;
};
