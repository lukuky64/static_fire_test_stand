#pragma once

#include "Arduino.h"
#include "esp_log.h"

class LoadCell
{
public:
    LoadCell();
    ~LoadCell();
    bool isReady();
    bool calibrate();

private:
    static constexpr const char *TAG = "LoadCell";
};
