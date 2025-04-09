// LoRaComm.h
#ifndef LoRaComm_h
#define LoRaComm_h

#include <Arduino.h>
#include <RH_RF95.h>

#include "esp_log.h"

class LoRaComm {
 public:
  LoRaComm();
  void begin(int csPin, int intPin, float freqMHz);
  bool createMessage();
  void sendMessage();
  void sendMessage(const char *inputmsg);  // overloaded function
  String checkForReply();

 private:
  RH_RF95 *rf95;
  static const int MAX_INPUT_LENGTH = 128;
  char inputArray[MAX_INPUT_LENGTH];

  int INT_PIN;
  int CS_PIN;
  float RF95_FREQ;

  static constexpr const char *TAG = "LORA_COMM";
};

#endif
