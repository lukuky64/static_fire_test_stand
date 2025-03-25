#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>

#include "Pin_Defs.hpp"

#include "User_Interface.hpp"

class Devices
{
public:
    Devices();
    ~Devices();

    void begin();

private:
    TwoWire I2C_BUS;
    User_Interface UI;
};
