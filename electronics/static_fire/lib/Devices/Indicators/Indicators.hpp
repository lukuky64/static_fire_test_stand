#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "SemaphoreGuard.hpp"

#include "Arduino.h"
#include <vector>

class Indicators
{
public:
    Indicators();
    ~Indicators();

    void setup(uint8_t ledPin);

    bool controlLed(int duration);

    void showCriticalError();
    void showWarning();
    void showSuccess();
    void showAllGood();
    void showAllOff();

    bool checkStatus();
    bool checkStatusBuzzer();
    bool checkStatusLed();

private:
    SemaphoreHandle_t m_ledMutex; // Mutex for RGB LED

    uint8_t m_ledPin;

    static constexpr const char *TAG = "Indicators";
};