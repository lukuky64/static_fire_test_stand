#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire)
{
}

Devices::~Devices()
{
}

bool Devices::begin(unsigned int logSD, unsigned int logSerial)
{
    // begin all coms and devices
    I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);

    m_indicators.setup(INDICATOR_LED);
    UI.init(I2C_BUS);
    UI.begin();
    m_bms.init(VOLTAGE_SENSE);
    m_igniter.init(IGNITER_CONTROL, IGNITER_SENSE, IGNITER_ARMED);

    return true;
}

bool Devices::calibrateSeq()
{
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

bool Devices::indicateStatus()
{
    if ((m_LoadCell.isReady()) && (m_bms.getPercentage() > 25))
    {
        m_indicators.showAllGood();
        return true;
    }
    else
    {
        m_indicators.showWarning();
        UI.showError("devices");
        return false;
    }
}

void Devices::refreshStatusAll()
{
    return;
}