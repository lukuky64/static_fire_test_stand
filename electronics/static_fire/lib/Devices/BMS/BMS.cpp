#include "BMS.hpp"

BMS::BMS()
{
    // maybe better to set to zero but atleast the filter won't take as long to stabilize
    m_currentVoltage = m_battery.max_voltage;
    m_curentCapacity = m_battery.max_capacity;
    m_alpha = 0.5;
}

BMS::~BMS()
{
    //
}

void BMS::init(uint8_t voltagePin)
{
    m_voltageSensePin = voltagePin;
    pinMode(m_voltageSensePin, INPUT);
    // analogSetAttenuation(ADC_11db); // 150 mV ~ 2450 mV, attenuation is 11dB by default so won't need this line
    update();
}

void BMS::update()
{
    float newReading = analogReadMilliVolts(m_voltageSensePin) / 1000; // !!! test this
    ESP_LOGI(TAG, "New voltage reading: %f", newReading);

    m_currentVoltage = newReading * (1 - m_alpha) + (m_currentVoltage * m_alpha);
    m_curentCapacity = m_battery.max_capacity * m_currentVoltage / m_battery.max_voltage;
}

float BMS::getVoltage()
{
    return m_currentVoltage;
}

float BMS::getCapacity()
{
    return m_curentCapacity;
}

float BMS::getPercentage()
{
    return m_curentCapacity / m_battery.max_capacity * 100;
}

BMS::Battery BMS::getBattery()
{
    return m_battery;
}