#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Pin_Defs.hpp"

#include "bitMaps.hpp"

struct mainPageStatus
{
    bool cell;
    bool sd;
    bool rf;
    bool armed;
    bool ready;
    float battPer;
};

class User_Interface
{
public:
    User_Interface();
    ~User_Interface();

    bool init(TwoWire &I2C_Bus);
    void begin();

    void drawBitMap(Bitmap data, int16_t x = -1, int16_t y = -1);
    void drawPageBar(bool cell, bool sd, bool rf, bool armed, bool ready, float battPer, bool forceUpdateAll);
    void drawForce(float forceInput);

    void drawIntroPage();

    void showSuccess(String msg);
    void showError(String msg);

private:
    const int SCREEN_WIDTH = 128; // OLED display width, in pixels
    const int SCREEN_HEIGHT = 64; // OLED display height, in pixels

    const int OLED_RESET = -1;       // Reset pin # (or -1 if sharing Arduino reset pin)
    const int SCREEN_ADDRESS = 0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

    Adafruit_SSD1306 display;

    mainPageStatus m_mainPageStatus{false, false, false, false, false, 0};
};
