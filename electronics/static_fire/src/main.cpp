#include <Arduino.h>

#include "Devices.hpp"

Devices devices;

void setup()
{
    Serial.begin(115200);
    delay(100);
    devices.begin();

    Serial.println("Setup complete!");
}

void loop()
{
    delay(100);
}
