#pragma once

#include <Arduino.h>

#include <functional>  // For std::function

#include "esp_log.h"

// #if ARDUINO_USB_CDC_ON_BOOT
// #define COMM_INTERFACE USBSerial
// #else
// #define COMM_INTERFACE Serial
// #endif

// extern USBCDC USBSerial;  // Declare USBSerial globally

#define COMM_INTERFACE USBSerial

class Commander {
 public:
  using CommandCallback = std::function<void(const char *)>;

  explicit Commander(unsigned long baud = 115200);

  void init();

  void addCommand(char cmdID, CommandCallback callback);
  void run();

  // Helper to parse and set a variable dynamically
  void setVariable(const char *arg);

 private:
  struct Command {
    char id;
    CommandCallback callback;
  };

  static const int MAX_COMMANDS = 10;
  static const int MAX_BUFFER_SIZE = 64;

  Command _commandList[MAX_COMMANDS];
  int _commandCount = 0;

  char _rxBuffer[MAX_BUFFER_SIZE];
  int _rxIndex = 0;

  unsigned long m_baud;

  void handleBuffer();

  static constexpr const char *TAG = "Commander";
};
