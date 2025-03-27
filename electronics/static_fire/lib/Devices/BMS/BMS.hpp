#pragma once

#include "Arduino.h"

class BMS
{

    struct Battery
    {
        float max_voltage;  // volts
        float max_capacity; // mAh
    };

public:
    BMS();
    ~BMS();

    void init(uint8_t voltagePin);
    void update();
    float getVoltage();
    float getCapacity();
    float getPercentage();
    Battery getBattery();

private:
    uint8_t m_voltageSensePin;

    float m_currentVoltage;
    float m_curentCapacity;

    Battery m_battery{4.2, 2200}; // try find an actual 18650 battery datasheet

    float m_alpha; // complementary filter coefficient

    static constexpr const char *TAG = "BMS";
};
