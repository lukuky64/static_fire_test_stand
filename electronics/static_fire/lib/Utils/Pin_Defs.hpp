#pragma once

// UART
#define MCU_RX GPIO_NUM_44
#define MCU_TX GPIO_NUM_43

// I2C
#define I2C_SDA GPIO_NUM_1
#define I2C_SCL GPIO_NUM_2

// RF LoRa
#define RF_DIO GPIO_NUM_9

#define SPI_CS_RF GPIO_NUM_10
#define SPI_MOSI_RF GPIO_NUM_11
#define SPI_CLK_RF GPIO_NUM_12
#define SPI_MISO_RF GPIO_NUM_13

// SD Card
#define SD_CD GPIO_NUM_3 //            !!!!!! This is supposed to be 48, but LED is here on the DEV board

#define SPI_CLK_SD GPIO_NUM_35
#define SPI_MOSI_SD GPIO_NUM_36
#define SPI_MISO_SD GPIO_NUM_37
#define SPI_CS_SD GPIO_NUM_38

// BMS
#define VOLTAGE_SENSE GPIO_NUM_6

// Indicators
#define INDICATOR_LED GPIO_NUM_15

#define IGNITER_CONTROL GPIO_NUM_16
#define IGNITER_SENSE GPIO_NUM_4
#define IGNITER_ARMED GPIO_NUM_18