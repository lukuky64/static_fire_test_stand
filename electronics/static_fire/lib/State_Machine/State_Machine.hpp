#pragma once

#include "Arduino.h"
#include "Commander/Commander.hpp"
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
  FIRE = 5,
};

class State_Machine {
 public:
  static State_Machine *instance;  // Static instance pointer

  static void onTimerInterrupt();  // Static ISR
  void timerCallback();            // Actual class method
  void setupHardwareTimer(uint32_t frequency_hz, void (*callback)(void));

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
  void loRaSeq();
  void fireSequence();

  // void checkActivityTask();
  STATES getCurrentState();
  void setCurrentState(STATES state);
  void criticalErrorSeq();
  void indicationSeq();
  void filterDataSeq();

  void logSeq();
  String stateToString(STATES state);

  // FreeRTOS Tasks
  static void taskManagerTask(void *pvParameters);
  static void indicationTask(void *pvParameters);
  // static void refreshStatusTask(void *pvParameters);
  static void filterDataTask(void *pvParameters);
  static void logTask(void *pvParameters);
  static void idleTask(void *pvParameters);
  static void LoRaTask(void *pvParameters);

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

  TaskHandle_t m_filterDataTaskHandle = NULL;
  TaskHandle_t m_balanceTaskHandle = NULL;
  TaskHandle_t m_BLDCTaskHandle = NULL;

  TaskHandle_t m_logTaskHandle = NULL;

  TaskHandle_t m_cpuUsageTaskHandle = NULL;

  TaskHandle_t m_wifiTaskHandle = NULL;

  TaskHandle_t m_LoRaTaskHandle = NULL;

  static constexpr const char *TAG = "State_Machine";

  Commander m_commander = Commander();

  char *recFirePassword = "";

  float m_logPeriod_ms;
  float m_loraPeriod_ms;

  float m_filteredForce = 0.0f;

  // float alpha = 0.061f;
  // float filtered = 0.0f;
  // int downsample_counter = 0;
  // const int downsample_factor = 5;
};