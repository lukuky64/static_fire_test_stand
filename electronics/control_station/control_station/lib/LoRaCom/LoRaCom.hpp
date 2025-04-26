// LoRaCom.h
#ifndef LoRaCom_h
#define LoRaCom_h

#include <Arduino.h>
#include <RHSoftwareSPI.h>
#include <RH_RF95.h>

#include "esp_log.h"

class LoRaCom {
 public:
  LoRaCom();
  void begin(uint8_t CLK, uint8_t MISO, int8_t MOSI, uint8_t csPin,
             uint8_t intPin, float freqMHz);
  // bool createMessage();
  // void sendMessage();
  void sendMessage(const char *inputmsg);  // overloaded function
  String checkForReply();
  int16_t getRssi();

  bool getData(char *buffer, const size_t bufferSize, int *_rxIndex);

 private:
  RH_RF95 *rf95;
  RHSoftwareSPI RH_SPI;

  static const int MAX_INPUT_LENGTH = 128;
  char inputArray[MAX_INPUT_LENGTH];

  int INT_PIN;
  int CS_PIN;
  float RF95_FREQ;

  static constexpr const char *TAG = "LORA_COMM";
};

#endif
