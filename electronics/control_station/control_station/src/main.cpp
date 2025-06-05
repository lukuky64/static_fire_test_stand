#include <Arduino.h>

#include "LoRaCom.hpp"
#include "SerialCom.hpp"

LoRaCom m_LoRaCom;
SerialCom m_serialCom;

#define SPI_CS_RF 10
#define SPI_MOSI_RF 11
#define SPI_CLK_RF 12
#define SPI_MISO_RF 13
#define RF_DIO 14

unsigned long lastMillis = 0;
unsigned long RSSI_interval = 2000;

void setup() {
  m_serialCom.init(115200);  // Initialize serial communication
  m_LoRaCom.begin(SPI_CLK_RF, SPI_MISO_RF, SPI_MOSI_RF, SPI_CS_RF, RF_DIO,
                  915.0f, 20);

  delay(1000);
  m_serialCom.sendData("Setup finished");
}

void loop() {
  char buffer[128];  // Buffer to store incoming data
  int rxIndex = 0;   // Index to track the length of the received message

  // Check for incoming data from the serial interface
  if (m_serialCom.getData(buffer, sizeof(buffer), &rxIndex)) {
    ESP_LOGI("SerialCom", "Received: %s", buffer);  // Log the received data
    m_LoRaCom.sendMessage(buffer);  // Send the received data over LoRa
  }

  // Check for incoming data from the LoRa interface
  if (m_LoRaCom.getData(buffer, sizeof(buffer), &rxIndex)) {
    // ESP_LOGI("LoRaCom", "Received: %s", buffer);  // Log the received data
    m_serialCom.sendData(buffer);  // Send the received data over serial
    m_serialCom.sendData("\n");    // Send the received data over serial
  }

  if (millis() - lastMillis >= RSSI_interval) {
    lastMillis = millis();
    int16_t rssi = m_LoRaCom.getRssi();
    // if (rssi != -1) {  // Check if the RSSI value is valid
    String msg = "DOWNLINK_RSSI = " + String(rssi) + "\n";
    m_serialCom.sendData(msg.c_str());
    // }
  }
}
