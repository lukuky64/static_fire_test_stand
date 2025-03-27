#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>

// #include "Log/Log.hpp"
#include "Indicators/Indicators.hpp"

#include "BMS/BMS.hpp"

#include "Igniter/Igniter.hpp"

#include "Pin_Defs.hpp"

#include "User_Interface.hpp"

#include "LoadCell/LoadCell.hpp"

class Devices
{
public:
    Devices();
    ~Devices();

    bool begin(unsigned int logSD, unsigned int logSerial);
    bool indicateStatus();
    void refreshStatusAll();
    bool sleepMode();
    void wakeMode();

    bool calibrateSeq();

public:
    // Log m_logger;
    Indicators m_indicators;
    User_Interface UI;
    BMS m_bms;
    Igniter m_igniter;
    LoadCell m_LoadCell;

private:
    TwoWire I2C_BUS;
};
