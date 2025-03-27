#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire)
{
}

Devices::~Devices()
{
}

bool Devices::begin()
{
    m_indicators.setup(INDICATOR_LED);

    // begin all coms and devices
    I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);
    UI.init(I2C_BUS);
    UI.begin();

    m_bms.init(VOLTAGE_SENSE);

    m_igniter.init(IGNITER_CONTROL, IGNITER_SENSE, IGNITER_ARMED);

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