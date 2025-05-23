#include "MCP4561_DIGI_POT.hpp"

#include <Wire.h>

#include "Arduino.h"

// Status Register Bit Constants. Note bits D4:D8 are reserved and set to '1'
#define MCP4561_SR_BIT0_WP ((uint8_t)0x00)
#define MCP4561_SR_BIT1_WL0 ((uint8_t)0x01)
#define MCP4561_SR_BIT2_WL1 ((uint8_t)0x02)  // Not applicable for the MCP4561
#define MCP4561_SR_BIT3_EEWA ((uint8_t)0x03)

// Terminal control (TCON) Register Bit Constants
#define MCP4561_TCON_R0B ((uint8_t)0x00)
#define MCP4561_TCON_R0W ((uint8_t)0x01)
#define MCP4561_TCON_R0A ((uint8_t)0x02)
#define MCP4561_TCON_R0HW ((uint8_t)0x03)
#define MCP4561_TCON_R1B ((uint8_t)0x04)   // Not applicable for the MCP4561
#define MCP4561_TCON_R1W ((uint8_t)0x05)   // Not applicable for the MCP4561
#define MCP4561_TCON_R1A ((uint8_t)0x06)   // Not applicable for the MCP4561
#define MCP4561_TCON_R1HW ((uint8_t)0x07)  // Not applicable for the MCP4561
#define MCP4561_TCON_GCEN ((uint8_t)0x08)

#define MCP4561_TCON_GCEN_BIT ((uint16_t)0x0100)

#define MCP4561_CMD_BYTE_WRITE_DATA ((uint8_t)0b00000000)
#define MCP4561_CMD_BYTE_INCREMENT ((uint8_t)0b00000100)
#define MCP4561_CMD_BYTE_DECREMENT ((uint8_t)0b00001000)
#define MCP4561_CMD_BYTE_READ_DATA ((uint8_t)0b00001100)

#define MCP4561_SET_BIT(p, whichBit) ((p) |= (1 << (whichBit)))
#define MCP4561_CLEAR_BIT(p, whichBit) ((p) &= ~((1) << (whichBit)))
#define MCP4561_TOGGLE_BIT(p, whichBit) ((p) ^= (1 << (whichBit)))
#define MCP4561_BIT_IS_SET(p, whichBit) ((p) & (1 << (whichBit)))

#define MCP4561_MAX_REG_VAL \
  ((uint16_t)0x01FF)  // See Table 7-2, Page pp56, note 1

MCP4561::MCP4561() {}

void MCP4561::init(uint8_t u8address, TwoWire &I2C_Bus) {
  m_I2C_Bus = &I2C_Bus;
  _u8address = u8address;
}

MCP4561::~MCP4561() {}  // Destructor, not used

uint16_t MCP4561::potConnectAll(uint8_t u8WhichPot) {
  uint16_t u16TCON = 0x0000;
  u16TCON = readVal(MCP4561_TCON_REG);
  switch (u8WhichPot) {
    case MCP4561_WIPER_0:
      u16TCON |= (MCP4561_TERM_R0B | MCP4561_TERM_R0W | MCP4561_TERM_R0A);
      // u16TCON = MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R0B) |
      // MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R0A) |
      // MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R0W);
      break;
    case MCP4561_WIPER_1:
      u16TCON |= (MCP4561_TERM_R1B | MCP4561_TERM_R1W | MCP4561_TERM_R1A);
      // u16TCON = MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R1B) |
      // MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R1A) |
      // MCP4561_SET_BIT(u16TCON_val,MCP4561_TCON_R1W);
      break;
    default:
      return MCP4561_ERR;
  }
  return writeVal(MCP4561_TCON_REG, u16TCON);
}

uint16_t MCP4561::potDisconnectAll(uint8_t u8WhichPot) {
  uint16_t u16TCON = 0x0000;
  u16TCON = readVal(MCP4561_TCON_REG);
  switch (u8WhichPot) {
    case MCP4561_WIPER_0:
      u16TCON &= ~(MCP4561_TERM_R0B | MCP4561_TERM_R0W | MCP4561_TERM_R0A);
      // u16TCON = MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R0B) |
      // MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R0A) |
      // MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R0W);
      break;
    case MCP4561_WIPER_1:
      u16TCON &= ~(MCP4561_TERM_R1B | MCP4561_TERM_R1W | MCP4561_TERM_R1A);
      // u16TCON = MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R1B) |
      // MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R1A) |
      // MCP4561_CLEAR_BIT(u16TCON_val,MCP4561_TCON_R1W);
      break;
    default:
      return MCP4561_ERR;
  }
  return writeVal(MCP4561_TCON_REG, u16TCON);
}

uint16_t MCP4561::potConnectSelective(uint16_t u16WhichTerminals) {
  uint16_t u16TCON = 0x0000;
  u16TCON = readVal(MCP4561_TCON_REG);
  u16TCON |= u16WhichTerminals;
  return writeVal(MCP4561_TCON_REG, u16TCON);
}

uint16_t MCP4561::potDisconnectSelective(uint16_t u16WhichTerminals) {
  uint16_t u16TCON = 0x0000;
  u16TCON = readVal(MCP4561_TCON_REG);
  u16TCON &= ~u16WhichTerminals;
  return writeVal(MCP4561_TCON_REG, u16TCON);
}

uint16_t MCP4561::readVal(uint8_t u8WhichRegister) {
  uint8_t u8LowByte = 0x00;
  uint8_t u8HighByte = 0x00;
  uint8_t u8CmdByte =
      (uint8_t)((u8WhichRegister << 4) | MCP4561_CMD_BYTE_READ_DATA);

  if (u8WhichRegister > MCP4561_GP_EEPROM_9) return MCP4561_ERR;

  m_I2C_Bus->beginTransmission(_u8address);  // Identify I2C device on bus
  m_I2C_Bus->write(u8CmdByte);               // Point to register if interest
  // m_I2C_Bus->endTransmission();
  m_I2C_Bus->endTransmission();
  m_I2C_Bus->requestFrom(_u8address,
                         (uint8_t)2);  // Request two bytes (to be concatonated)
  delay(1);
  if (m_I2C_Bus->available()) {
    u8HighByte = m_I2C_Bus->read();  // read higher byte
    u8LowByte = m_I2C_Bus->read();   // read lower byte
    return (uint16_t)((((uint16_t)(u8HighByte << 8)) | u8LowByte) & 0x01FF);
  } else
    return MCP4561_ERR;
}

uint16_t MCP4561::writeVal(uint8_t u8WhichRegister, uint16_t u16Value) {
  uint8_t u8CmdByte = MCP4561_CMD_BYTE_WRITE_DATA;

  if ((u8WhichRegister > MCP4561_GP_EEPROM_9) ||
      (u16Value > MCP4561_MAX_REG_VAL))
    return MCP4561_ERR;

  // Command byte format see Figure 7-1 pp55. B7   B6   B5   B4   B3  B2  B1  B0
  //                                          AD3, AD2, AD1, AD0, C1, C0, D9, D8
  u8CmdByte |= ((u8WhichRegister << 4) & 0b11110000) |
               (((u16Value & 0x0300) >> 8) & 0b00000011);
  m_I2C_Bus->beginTransmission(_u8address);  // Identify I2C device on bus

  m_I2C_Bus->write(u8CmdByte);
  m_I2C_Bus->write(((uint8_t)(u16Value & 0xFF)));
  uint8_t error = m_I2C_Bus->endTransmission();
  m_I2C_Bus->flush();

  if (error == 0) {
    return MCP4561_SUCCESS;
  }

  return MCP4561_ERR;
}
