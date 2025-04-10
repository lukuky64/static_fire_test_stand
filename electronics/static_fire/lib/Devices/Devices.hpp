#pragma once

#include <SPI.h>
#include <Wire.h>

#include "./Params.hpp"
#include "Arduino.h"
#include "BMS/BMS.hpp"
#include "Igniter/Igniter.hpp"
#include "Indicators/Indicators.hpp"
#include "LoRaCom/LoRaCom.hpp"
#include "LoadCell/LoadCell.hpp"
#include "Log/Log.hpp"
#include "Pin_Defs.hpp"
#include "SerialCom/SerialCom.hpp"
#include "User_Interface.hpp"

class Devices {
 public:
  Devices();
  ~Devices();

  bool begin();
  bool checkStatus();
  void refreshStatusAll();
  bool sleepMode();
  void wakeMode();

  bool calibrate();

  void aquireData();

 public:
  // Log m_logger;
  Indicators m_indicators;
  User_Interface UI;
  BMS m_bms;
  Igniter m_igniter;
  LoadCell m_LoadCell;
  Log m_logger;
  LoRaCom m_LoRaCom;
  SerialCom m_serialCom;

 private:
  TwoWire I2C_BUS;
  SPIClass SPI_SD_BUS;
  SPIClass SPI_RF_BUS;

  const float m_criticalBattery = 20;  // 20% battery level
};
