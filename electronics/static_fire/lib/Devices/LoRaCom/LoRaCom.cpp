// LoRaCom.cpp
#include "LoRaCom.hpp"

// Issues:
// 1. If two devices transmit at the exact same time, neither message will be
// received

LoRaCom::LoRaCom() {}

void LoRaCom::begin(uint8_t CLK, uint8_t MISO, int8_t MOSI, uint8_t csPin,
                    uint8_t intPin, float freqMHz) {
  RH_SPI.setPins(MISO, MOSI, CLK);
  rf95 = new RH_RF95(csPin, intPin, RH_SPI);
  INT_PIN = intPin;
  CS_PIN = csPin;
  RF95_FREQ = freqMHz;

  if (!rf95->init()) {
    ESP_LOGE(TAG, "LoRa radio init failed");
    while (1);
  }

  rf95->setFrequency(RF95_FREQ);
  // If using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin,
  // then you can set transmitter powers from 5 to 20 dBm:
  rf95->setTxPower(10, false);  // higher = longer range. !!! was 20, but if we
                                // can get away with lower then that is better
  rf95->setSpreadingFactor(7);  // 6-12 -> higher = longer range
  // rf95->setSignalBandwidth(7800); // 7.8k to 500k. lower = longer range
  // rf95->setCodingRate4(8);        // 5-8 higher = longer range
  // rf95->setPreambleLength(1024);  // 6-65535 higher = longer range

  // modem config choice
  // rf95->setModemConfig(RH_RF95::Bw31_25Cr48Sf512); // slow and long range
  ESP_LOGI(TAG, "LoRa radio init successful!");
}

// bool LoRaCom::createMessage() {
//   bool messageCreated = false;
//   if (Serial.available() > 0) {
//     memset(inputArray, 0, MAX_INPUT_LENGTH);  // Clear the buffer
//     int Index = 0;

//     while (Serial.available() > 0 && Index < MAX_INPUT_LENGTH - 1) {
//       char incomingByte = Serial.read();
//       if (incomingByte == '\n') {
//         inputArray[Index] = '\0';
//         messageCreated = true;
//         break;
//       } else {
//         inputArray[Index] = incomingByte;
//         Index++;
//         vTaskDelay(pdMS_TO_TICKS(5));  // this fixes reliablity issues with
//         the data somehow
//       }
//     }
//   }

//   return messageCreated;
// }

// void LoRaCom::sendMessage() {
//   ESP_LOGE(TAG, "Trying to send message");
//   if (inputArray[0] != '\0') {  // Check if the message is not empty
//     ESP_LOGI(TAG, "Transmit: ");
//     ESP_LOGI(TAG, inputArray);
//     rf95->send((uint8_t *)inputArray, strlen(inputArray));
//     vTaskDelay(pdMS_TO_TICKS(10));
//     rf95->waitPacketSent();
//     vTaskDelay(pdMS_TO_TICKS(10));
//   }
// }

void LoRaCom::sendMessage(const char *inputmsg) {
  if (inputmsg[0] != '\0') {  // Check the message is not empty
    ESP_LOGI(TAG, "Transmitting [%s]", inputmsg);
    rf95->send((uint8_t *)inputmsg, strlen(inputmsg));
    vTaskDelay(pdMS_TO_TICKS(10));
    rf95->waitPacketSent();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// return message
String LoRaCom::checkForReply() {
  String message =
      "";  // Initialize an empty String to store the received message
  if (rf95->available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95->recv(buf, &len)) {
      buf[len] = '\0';                // Ensure null-termination
      message = String((char *)buf);  // Convert buf to a String and store it

      ESP_LOGI(TAG, "Received [%s]", message.c_str());
      ESP_LOGI(TAG, "\t RSSI [%d]", rf95->lastRssi());
      ESP_LOGI(TAG, "\t SNR [%d]", rf95->lastSNR());
    } else {
      ESP_LOGE(TAG, "Receive failed");
    }
  }
  return message;  // Return the message received or an empty string if no
                   // message was received
}

bool LoRaCom::getData(char *buffer, const size_t bufferSize, int *_rxIndex) {
  // checkForReply !!! need to implement
  return false;
}
