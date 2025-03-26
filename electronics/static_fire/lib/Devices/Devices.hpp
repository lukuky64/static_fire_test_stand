#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>

// #include "Log/Log.hpp"
// #include "Indicators/Indicators.hpp"

#include "Pin_Defs.hpp"

#include "User_Interface.hpp"

class Devices
{
public:
    Devices();
    ~Devices();

    bool begin();
    bool indicateStatus();
    void refreshStatusAll();
    bool sleepMode();
    void wakeMode();

    bool calibrateSeq();

    bool init(unsigned int LOG_SD, unsigned int LOG_SERIAL);

public:
    // Log m_logger;
    // Indicators m_indicators;

private:
    TwoWire I2C_BUS;
    User_Interface UI;
};
