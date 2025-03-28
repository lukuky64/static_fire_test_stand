#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire)
{
}

Devices::~Devices()
{
}

bool Devices::begin()
{
    // begin all coms and devices
    I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);
    SPI_SD_BUS.begin(SPI_CLK_SD, SPI_MISO_SD, SPI_MOSI_SD);

    m_indicators.setup(INDICATOR_LED);
    UI.init(I2C_BUS);
    UI.begin();
    m_igniter.init(IGNITER_CONTROL, IGNITER_SENSE, IGNITER_ARMED);
    m_LoadCell.init(I2C_BUS, LOAD_SENSE);

    if (Params::LOG_SD == 1)
    {
        m_logger.selectLogSD(SPI_SD_BUS); // priority to SD
    }
    else if (Params::LOG_SERIAL == 1)
    {
        m_logger.selectLogSerial();
    }

    bool bmsStatus = m_bms.init(VOLTAGE_SENSE);

    bool success = bmsStatus;

    return success;
}

bool Devices::calibrate()
{
    bool loadCellCalibrated = m_LoadCell.calibrate();

    bool somethingElse = true;

    return loadCellCalibrated && somethingElse;
}

bool Devices::sleepMode()
{
    return true;
}
void Devices::wakeMode()
{
    return;
}

bool Devices::checkStatus()
{
    bool loadCellReady = m_LoadCell.isReady();
    float bmsPercentage = m_bms.getPercentage();
    bool sdCardReady = m_logger.m_sdTalker.checkPresence();
    bool rf_connected = false;
    bool systemArmed = m_igniter.sytemArmed();

    bool allGood = loadCellReady && (bmsPercentage > 20);

    UI.drawPageBar(loadCellReady, sdCardReady, rf_connected, systemArmed, allGood, bmsPercentage, false);

    if (allGood)
    {
        if (sdCardReady)
        {
            m_logger.startNewLog();
        }

        m_indicators.showAllGood();
        return true;
    }
    else
    {
        m_indicators.showWarning();
        UI.showError("device status");
        return false;
    }
}

void Devices::aquireData()
{
    // ESP_LOGI("Devices", "Aquiring Data!");
}