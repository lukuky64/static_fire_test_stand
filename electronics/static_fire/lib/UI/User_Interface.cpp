#include "User_Interface.hpp"

User_Interface::User_Interface()
{
}

User_Interface::~User_Interface()
{
}

bool User_Interface::init(TwoWire &I2C_Bus)
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_Bus, OLED_RESET);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }

    return true;
}

void User_Interface::begin()
{
    // Clear the buffer
    display.clearDisplay();
    display.display();
    delay(500);

    // // Draw a single pixel in white
    // display.drawPixel(10, 10, SSD1306_WHITE);
    // display.display();
    // delay(500);
    drawBitMap();
}

void User_Interface::drawBitMap()
{
    display.clearDisplay();

    display.drawBitmap(
        (display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2,
        logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();

    for (int i = 0; i < 10; i++)
    {
        delay(100);
        display.invertDisplay(true);
        delay(100);
        display.invertDisplay(false);
    }
    delay(1000);
}