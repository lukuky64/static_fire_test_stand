#pragma once

#include "Arduino.h"
#include "Params.hpp"
#include "SD_Talker/SD_Talker.hpp"
#include "Serial_Talker/Serial_Talker.hpp"
#include "esp_log.h"
#include "pin_defs.hpp"

class Log {
 public:
  Log();
  ~Log();
  bool selectLogSD(SPIClass &SPI_BUS);
  void selectLogSerial();
  // bool beginSerial();
  // bool beginSD();
  bool startNewLog();
  bool checkStartLog();

  // bool Logger(int len);
  bool logData(const float *data, int dataSize);
  void setStartTime();
  void forceFlush();

  bool writeBufferAll();

  bool isLogSetup();

  SD_Talker m_sdTalker;
  Serial_Talker m_serialTalker;

 private:
  bool m_sdLog;
  bool m_serialLog;
  unsigned long m_startTime;

  static constexpr size_t m_bufferSize = 4096;  // ensure mulitple of 512 bytes
  char charBuffer[m_bufferSize];
  size_t currentBufferPos;  // Track current position in buffer

  static constexpr const char *TAG = "Log";
};