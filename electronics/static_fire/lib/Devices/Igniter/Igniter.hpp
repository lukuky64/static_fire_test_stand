#pragma once

#include "Arduino.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freeRTOS/task.h"

class Igniter
{
public:
    Igniter();
    ~Igniter();

    void sendIgnition(String password);
    void init(uint8_t controlPin, uint8_t sensePin, uint8_t armedPin);
    bool igniterFaulty();
    bool sytemArmed();

private:
    uint8_t m_igniterControlPin;
    uint8_t m_igniterSensePin;
    uint8_t m_igniterArmedPin;

    static constexpr const char *TAG = "Igniter";

    const String m_password = "YES";

    float m_alpha = 0.5; // analog filter coefficient
};
