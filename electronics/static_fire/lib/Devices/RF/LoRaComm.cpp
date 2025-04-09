// LoRaComm.cpp
#include "LoRaComm.hpp"

// Issues:
// 1. If two devices transmit at the exact same time, neither message will be
// received

LoRaComm::LoRaComm() {}

void LoRaComm::begin(int csPin, int intPin, float freqMHz) {
  rf95 = new RH_RF95(csPin, intPin);
  INT_PIN = intPin;
  CS_PIN = csPin;
  RF95_FREQ = freqMHz;

  if (!rf95->init()) {
    ESP_LOGE(TAG, "LoRa radio init failed");
    while (1);
  }

  Serial.println("LoRa radio init successful!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95->setFrequency(RF95_FREQ)) {
    Serial.println("set Frequency failed");
    while (1);
  }

  // If using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin,
  // then you can set transmitter powers from 5 to 20 dBm:
  rf95->setTxPower(20, false);  // higher = longer range
  rf95->setSpreadingFactor(7);  // 6-12 -> higher = longer range
  // rf95->setSignalBandwidth(7800); // 7.8k to 500k. lower = longer range
  // rf95->setCodingRate4(8);        // 5-8 higher = longer range
  // rf95->setPreambleLength(1024);  // 6-65535 higher = longer range

  // modem config choice
  // rf95->setModemConfig(RH_RF95::Bw31_25Cr48Sf512); // slow and long range
}

bool LoRaComm::createMessage() {
  bool messageCreated = false;
  if (Serial.available() > 0) {
    memset(inputArray, 0, MAX_INPUT_LENGTH);  // Clear the buffer
    int Index = 0;

    while (Serial.available() > 0 && Index < MAX_INPUT_LENGTH - 1) {
      char incomingByte = Serial.read();
      if (incomingByte == '\n') {
        inputArray[Index] = '\0';
        messageCreated = true;
        break;
      } else {
        inputArray[Index] = incomingByte;
        Index++;
        delay(5);  // this fixes reliablity issues with the data somehow
      }
    }
  }

  return messageCreated;
}

void LoRaComm::sendMessage() {
  Serial.println("Trying to send message");
  if (inputArray[0] != '\0') {  // Check if the message is not empty
    Serial.print("Transmit: ");
    Serial.println(inputArray);
    rf95->send((uint8_t *)inputArray, strlen(inputArray));
    delay(10);
    rf95->waitPacketSent();
    delay(10);
  }
}

void LoRaComm::sendMessage(const char *inputmsg) {
  if (inputmsg[0] != '\0') {  // Check if the message is not empty
    Serial.print("Transmit: ");
    Serial.println(inputmsg);
    rf95->send((uint8_t *)inputmsg, strlen(inputmsg));
    delay(10);
    rf95->waitPacketSent();
    delay(10);
  }
}

// return message
String LoRaComm::checkForReply() {
  String message =
      "";  // Initialize an empty String to store the received message
  if (rf95->available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95->recv(buf, &len)) {
      buf[len] = '\0';                // Ensure null-termination
      message = String((char *)buf);  // Convert buf to a String and store it

      // Continue to print details to the Serial Monitor
      Serial.print("Received: ");
      Serial.print(message);  // Print the message
      Serial.print("\t RSSI: ");
      Serial.print(rf95->lastRssi(), DEC);  // Print RSSI value
      Serial.print("\t SNR: ");
      Serial.println(rf95->lastSNR(),
                     DEC);  // Print SNR value, then move to new line
    } else {
      Serial.println("Receive failed");
    }
  }
  return message;  // Return the message received or an empty string if no
                   // message was received
}
