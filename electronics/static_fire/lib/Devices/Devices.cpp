#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire)
{
}

Devices::~Devices()
{
}

void Devices::begin()
{
    // begin all coms
    I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);
    UI.init(I2C_BUS);

    UI.begin();
}
