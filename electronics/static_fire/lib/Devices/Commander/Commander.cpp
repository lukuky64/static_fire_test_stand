#include "Commander.hpp"

#include "params.hpp"

// USBCDC USBSerial(0);  // Define USBSerial once here

Commander::Commander() {}

void Commander::init(SerialCom *serialCom, LoRaCom *loraCom) {
  m_loraCom = loraCom;
  m_serialCom = serialCom;
}

void Commander::addCommand(char cmdID, CommandCallback callback) {
  // Prevent overflow of the command array
  if (_commandCount < MAX_COMMANDS) {
    _commandList[_commandCount].id = cmdID;
    _commandList[_commandCount].callback = callback;
    _commandCount++;

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Added command [%c]", cmdID);
    m_serialCom->sendData(buffer);

  } else {
    ESP_LOGE(TAG, "Command list is full! Cannot add command [%c]", cmdID);
  }
}

void Commander::run() {
  // Read all available characters
  if (!m_serialCom->getData(_rxBuffer, sizeof(_rxBuffer), &_rxIndex)) {
    if (!m_loraCom->getData(_rxBuffer, sizeof(_rxBuffer), &_rxIndex)) {
      return;
    }
  }

  handleBuffer();
  _rxIndex = 0;  // Reset index after processing
}

void Commander::handleBuffer() {
  // If no characters, do nothing
  if (_rxIndex == 0) return;

  // The first character in _rxBuffer is the command ID
  char cmdID = _rxBuffer[0];

  // The rest of the string (after the first character) is the command argument
  const char *arg = (_rxIndex > 1) ? &_rxBuffer[1] : "";

  // Search for a matching command
  for (int i = 0; i < _commandCount; i++) {
    if (_commandList[i].id == cmdID) {
      // If found, call its callback with the argument
      _commandList[i].callback(arg);
      ;
      return;
    }
  }

  // If no match, print an error
  COMM_INTERFACE.print("Unknown command: ");
  COMM_INTERFACE.println(cmdID);
}

void Commander::setVariable(const char *arg) {
  //   const char *delimiter = strchr(arg, '=');
  //   if (!delimiter) {
  //     COMM_INTERFACE.println("Invalid format. Use varName=value");
  //     return;
  //   }

  //   String varName = String(arg).substring(0, delimiter - arg);
  //   String valueStr = String(arg).substring(delimiter - arg + 1);

  //   // Detect float vs. integer by looking for a decimal point
  //   if (valueStr.indexOf('.') >= 0) {
  //     float fValue = valueStr.toFloat();
  //     Params::savePreference(varName.c_str(), fValue);
  //     COMM_INTERFACE.print("Stored float preference: ");
  //   } else {
  //     unsigned int iValue = (unsigned int)valueStr.toInt();
  //     Params::savePreference(varName.c_str(), iValue);
  //     COMM_INTERFACE.print("Stored integer preference: ");
  //   }

  //   COMM_INTERFACE.print(varName);
  //   COMM_INTERFACE.print(" = ");
  //   COMM_INTERFACE.println(valueStr);
}