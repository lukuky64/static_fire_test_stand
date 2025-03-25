#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Pin_Defs.hpp"

#include "logo.hpp"

class User_Interface
{
public:
    User_Interface();
    ~User_Interface();

    bool init(TwoWire &I2C_Bus);
    void begin();

    void drawBitMap();

private:
    const int SCREEN_WIDTH = 128; // OLED display width, in pixels
    const int SCREEN_HEIGHT = 64; // OLED display height, in pixels

    const int OLED_RESET = -1;       // Reset pin # (or -1 if sharing Arduino reset pin)
    const int SCREEN_ADDRESS = 0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

    Adafruit_SSD1306 display;
};
