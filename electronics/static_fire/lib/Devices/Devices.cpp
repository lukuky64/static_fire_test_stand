#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire)
{
}

Devices::~Devices()
{
}

bool Devices::begin()
{
    // begin all coms
    I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);
    UI.init(I2C_BUS);

    UI.begin();

    return true;
}

bool Devices::sleepMode()
{
    return true;
}
void Devices::wakeMode()
{
    return;
}