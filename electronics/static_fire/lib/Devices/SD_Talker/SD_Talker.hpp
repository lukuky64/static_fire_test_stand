#pragma once

#include "Arduino.h"
#include "esp_log.h"
#include <SD.h>
#include "SemaphoreGuard.hpp"

class SD_Talker
{
public:
    SD_Talker();
    ~SD_Talker();

#if DUMMY_SD

    bool checkStatus() { return true; }
    bool begin(uint8_t CS, SPICOM &SPI_BUS) { return true; }
    bool createFile(String StartMsg, String prefix) { return true; }
    bool writeToBuffer(String dataString) { return true; }
    void flushBuffer();
    bool isInitialized() { return true; }
    String createUniqueLogFile(String prefix) { return "true"; }
    bool createNestedDirectories(String prefix) { return true; }
    bool checkPresence() { return true; }

#else
    bool checkStatus();
    bool begin(uint8_t cardDetect, uint8_t CS, SPIClass &SPI_BUS);

    bool createFile(String StartMsg, String prefix);

    // bool writeToBuffer(String dataString);
    bool writeBuffer(const char *buffer, size_t bufferIndex);

    String createUniqueLogFile(String prefix);
    bool createNestedDirectories(String prefix);
    bool checkPresence();
    bool checkFileOpen();

private:
    File dataFile;
    String fileName;
    bool fileOpen;
    String buffer;
    bool initialised;

    uint8_t m_cardDetectPin;

    SPIClass *m_SPI_BUS = nullptr;
    uint8_t m_CS;

    bool sdWait(int timeout);

    static constexpr const char *TAG = "SD_Talker";

#endif
};