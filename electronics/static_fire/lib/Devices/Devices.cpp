#include "Devices.hpp"

Devices::Devices() : I2C_BUS(Wire) {}

Devices::~Devices() {}

bool Devices::begin() {
  // begin all coms and devices
  I2C_BUS.begin(I2C_SDA, I2C_SCL, 400000);

  m_indicators.setup(INDICATOR_LED);

  m_LoRaCom.begin(SPI_CLK_RF, SPI_MISO_RF, SPI_MOSI_RF, SPI_CS_RF, RF_DIO,
                  915.0f, 10);
  m_serialCom.init(115200);

  UI.init(I2C_BUS);
  UI.begin();
  m_igniter.init(IGNITER_CONTROL, IGNITER_SENSE, IGNITER_ARMED);
  m_loadCell.init(I2C_BUS, LOAD_SENSE);

  if (Params::LOG_SD == 1) {
    SPI_SD_BUS.begin(SPI_CLK_SD, SPI_MISO_SD, SPI_MOSI_SD);
    m_logger.selectLogSD(SPI_SD_BUS);
  } else if (Params::LOG_SERIAL == 1) {
    m_logger.selectLogSerial();
  }

  bool bmsStatus = m_bms.init(VOLTAGE_SENSE);

  bool success = bmsStatus;

  return success;
}

bool Devices::calibrate() {
  bool loadCellCalibrated = m_loadCell.calibrate();

  bool somethingElse = true;

  return loadCellCalibrated && somethingElse;
}

bool Devices::sleepMode() { return true; }
void Devices::wakeMode() { return; }

bool Devices::checkStatus() {
  bool loadCellReady = m_loadCell.isReady();
  float bmsPercentage = m_bms.getPercentage();
  bool sdCardReady = m_logger.m_sdTalker.checkPresence();
  bool rf_connected = false;
  bool systemArmed = m_igniter.sytemArmed();

  bool allGood = (bmsPercentage > m_criticalBattery);

  UI.drawPageBar(loadCellReady, sdCardReady, rf_connected, systemArmed, allGood,
                 bmsPercentage, false);

  // m_igniter.igniterReady();

  if (allGood) {
    if (sdCardReady) {
      m_logger.startNewLog();
    }

    m_indicators.showAllGood();
    return true;
  } else {
    m_indicators.showWarning();
    // String("Load:") + (loadCellReady ? "OK" : "FAIL") +
    String msg = String("\nBatt:") + String(uint8_t(bmsPercentage)) + "%";
    UI.showError(msg, 4000U);
    return false;
  }
}

void Devices::aquireData() {
  // ESP_LOGI("Devices", "Aquiring Data!");
}