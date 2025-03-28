#include "LoadCell.hpp"

LoadCell::LoadCell()
{
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
    float offset = 0.0;
    ESP_LOGI(TAG, "Load cell offset after calibration: %f", offset);
    return true;
}