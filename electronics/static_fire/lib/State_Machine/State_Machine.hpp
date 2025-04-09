#pragma once

#include "Arduino.h"
#include "Commander.hpp"
#include "Devices.hpp"
#include "FreeRTOSConfig.h"
#include "Params.hpp"
#include "SemaphoreGuard.hpp"
#include "Threading.hpp"
#include "TimerGuard.hpp"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freeRTOS/task.h"
#include "freertos/FreeRTOS.h"

enum STATES {
  INITIALISATION = 0,
  CRITICAL_ERROR = 1,
  CALIBRATION = 2,
  IDLE = 3,
  LIGHT_SLEEP = 4,
  CONTROL = 5,
};

class State_Machine {
 public:
  State_Machine();
  ~State_Machine();

  void begin();
  void loop();

  // void cpuUsageMonitorSeq();

  void setupCommands();

  void initialisationSeq();
  bool calibrationSeq();
  void lightSleepSeq();
  void idleSeq();
  // void checkActivityTask();
  STATES getCurrentState();
  void setCurrentState(STATES state);
  void criticalErrorSeq();
  void indicationSeq();

  void logSeq();
  const char *stateToString(STATES state);

  // FreeRTOS Tasks
  static void taskManagerTask(void *pvParameters);
  static void indicationTask(void *pvParameters);
  static void refreshStatusTask(void *pvParameters);
  static void updateDataTask(void *pvParameters);
  static void logTask(void *pvParameters);
  static void idleTask(void *pvParameters);

 private:
  SemaphoreHandle_t m_stateMutex = NULL;

  // SemaphoreHandle_t m_controllableMutex = NULL;
  // bool m_controllable;

  STATES m_currState;
  Devices m_devices;

  // FreeRTOS Handles
  TaskHandle_t m_idleTaskHandle = NULL;
  TaskHandle_t m_taskManagerTaskHandle = NULL;
  TaskHandle_t m_indicationLoopTaskHandle = NULL;
  TaskHandle_t m_refreshStatusTaskHandle = NULL;

  TaskHandle_t m_updateDataTaskHandle = NULL;
  TaskHandle_t m_balanceTaskHandle = NULL;
  TaskHandle_t m_BLDCTaskHandle = NULL;

  TaskHandle_t m_logTaskHandle = NULL;

  TaskHandle_t m_cpuUsageTaskHandle = NULL;

  TaskHandle_t m_wifiTaskHandle = NULL;

  static constexpr const char *TAG = "State_Machine";

  Commander m_commander = Commander(115200);
};