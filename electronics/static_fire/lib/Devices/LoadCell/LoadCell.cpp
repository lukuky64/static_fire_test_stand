#include "LoadCell.hpp"

LoadCell::LoadCell()
{
}

void LoadCell::init(TwoWire &I2C_Bus, uint8_t sensorPin)
{
    m_digitalTrim.init(0x2E, I2C_Bus);

    if (m_digitalTrim.potConnectAll(MCP4561_WIPER_0 == MCP4561_SUCCESS))
    {
        ESP_LOGI(TAG, "Pot connected!");
    }
    else
    {
        ESP_LOGI(TAG, "Pot NOT connected!");
    }

    m_sensorPin = sensorPin;
    pinMode(m_sensorPin, INPUT);

    uint16_t value = 64; // set to midscale for initialisation
    if (m_digitalTrim.writeVal(MCP4561_VOL_WIPER_0, value) == MCP4561_SUCCESS)
    {
        m_currentPotPos = value;
        ESP_LOGI(TAG, "Set trim pos %d", value);
    }
    else
    {
        ESP_LOGE(TAG, "Could not set trim pos %d", value);
    }
}

LoadCell::~LoadCell()
{
}

bool LoadCell::isReady()
{
    return true;
}

bool LoadCell::calibrate()
{
    float offsetValmV = 0;

    uint16_t numReadings = 10;

    for (int i = 0; i < numReadings; i++)
    {
        float newSample = static_cast<float>(getSample_mV());
        offsetValmV = newSample * (1 - m_alpha) + (offsetValmV * m_alpha);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    ESP_LOGI(TAG, "Load cell offset after calibration: %f", offsetValmV);

    return trimRef(offsetValmV);
}

float LoadCell::getSample_mV()
{
    float newReading = analogReadMilliVolts(m_sensorPin); // !!! test this
    return 0.0f;
}

bool LoadCell::trimRef(float mV_offset)
{
    // other method is to use increment/decrement. this will be slower BUT we can implement feedback.

    // Max voltage=3.3, min=0, step=25.58mV
    static const float voltageRange = 3300.0f;
    static const float fullScale = 129.0f;
    static const float divisor = voltageRange / fullScale;

    int16_t signed_steps = static_cast<int16_t>(round(mV_offset / divisor)); // how many steps to get to value from current pos

    uint16_t newPos = m_currentPotPos + signed_steps;

    if (m_digitalTrim.writeVal(MCP4561_VOL_WIPER_0, newPos) == MCP4561_SUCCESS)
    {
        m_currentPotPos = newPos;
        ESP_LOGI(TAG, "Set trim pos %d", newPos);
        return true;
    }
    else
    {
        ESP_LOGI(TAG, "Cannot set trim pos %d", newPos);
        return false;
    }
}