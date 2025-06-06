#include "State_Machine.hpp"

#include "driver/timer.h"  // ESP-IDF timer driver

// #include "perfmon.hpp"

State_Machine *State_Machine::instance = nullptr;

State_Machine::State_Machine() { m_stateMutex = xSemaphoreCreateMutex(); }

State_Machine::~State_Machine() {}

void State_Machine::begin() {
  instance = this;  // Set static pointer to this instance

  // perfmon_start();

  setCurrentState(INITIALISATION);

  if (m_taskManagerTaskHandle == NULL) {
    ESP_LOGI(TAG, "Starting Task Manager Task");
    xTaskCreate(&State_Machine::taskManagerTask, "Starting Task Manager", 4096,
                this, PRIORITY_MEDIUM, &m_taskManagerTaskHandle);
  }
}

void State_Machine::setupHardwareTimer(uint32_t frequency_hz,
                                       void (*callback)(void)) {
  const timer_group_t timer_group = TIMER_GROUP_0;  // Use Timer Group 0
  const timer_idx_t timer_idx = TIMER_0;            // Use Timer 0

  timer_config_t config{};
  config.divider = 80;  // 1 tick = 1 microsecond
  config.counter_dir = TIMER_COUNT_UP;
  config.counter_en = TIMER_PAUSE;
  config.alarm_en = TIMER_ALARM_EN;
  config.auto_reload = TIMER_AUTORELOAD_EN;
  config.clk_src = TIMER_SRC_CLK_APB;

  timer_init(timer_group, timer_idx, &config);

  timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);

  uint64_t alarm_value_us =
      1000000ULL / frequency_hz;  // period in microseconds
  timer_set_alarm_value(timer_group, timer_idx, alarm_value_us);

  timer_enable_intr(timer_group, timer_idx);

  // Attach ISR
  timer_isr_callback_add(timer_group, timer_idx, (timer_isr_t)callback, NULL,
                         0);

  timer_start(timer_group, timer_idx);
}

void State_Machine::taskManagerTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));
  // Convert generic pointer back to State_Machine*
  auto *machine = static_cast<State_Machine *>(pvParameters);

  while (true) {
    machine->loop();

    // ESP_LOGI(TAG, "Current state: %s",
    // machine->stateToString(static_cast<STATES>(machine->getCurrentState())));
    // UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    // ESP_LOGI(TAG, "Remaining stack: %u words\n", uxHighWaterMark);

    vTaskDelay(
        pdMS_TO_TICKS(Params::TASK_MANAGER_MS));  // Loop current has blocking
                                                  // in certain functions
  }
}

void State_Machine::loop() {
  STATES currState = getCurrentState();

  switch (currState) {
    case INITIALISATION: {
      initialisationSeq();
    } break;
    case CRITICAL_ERROR: {
      criticalErrorSeq();
    } break;
    case CALIBRATION: {
      // !!! commenting out so i can just look at initialisaiton for now
      calibrationSeq();
    } break;
    case IDLE: {
      loRaSeq();
      filterDataSeq();
      logSeq();   // initial logging start
      idleSeq();  // this is a blocking function. fine for our case but not
                  // good for task manager

    } break;
    case LIGHT_SLEEP: {
      lightSleepSeq();
      indicationSeq();
      logSeq();  // go back to logging after sleep
    } break;
    case FIRE: {
      fireSequence();
    } break;
    default:
      break;
  }
}

void State_Machine::filterDataSeq() {
  if ((m_filterDataTaskHandle == NULL)) {
    xTaskCreate(&State_Machine::filterDataTask, "Starting filter Task", 4096,
                this, PRIORITY_HIGH, &m_filterDataTaskHandle);
  }
}

// void State_Machine::onTimerInterrupt() {
//   if (instance != nullptr) {
//     instance->timerCallback();
//   }
// }

// void State_Machine::timerCallback() {
//   float raw_reading = m_devices.m_loadCell.getForceSample();
//   filtered = alpha * raw_reading + (1.0f - alpha) * filtered;

//   if (++downsample_counter >= downsample_factor) {
//     m_filteredForce = filtered;
//     downsample_counter = 0;
//   }
// }

void State_Machine::filterDataTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));  // Initial delay

  auto *machine = static_cast<State_Machine *>(pvParameters);

  float alpha = 0.2f;  // Smoothing factor
  float filtered = 0.0f;
  float raw_reading = 0.0f;
  int downsample_counter = 0;
  const int downsample_factor = 5;  // Save every nth filtered sample

  while (true) {
    raw_reading = machine->m_devices.m_loadCell.getForceSample();
    filtered = alpha * raw_reading + (1.0f - alpha) * filtered;

    if (++downsample_counter >= downsample_factor) {
      machine->m_filteredForce = filtered;
      downsample_counter = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(Params::AQUISITION_MS));
  }
}

void State_Machine::fireSequence() {
  m_loraPeriod_ms = Params::LORACOM_MS;
  ESP_LOGI(TAG, "Fire sequence started");
  if (recFirePassword != "") {
    m_logPeriod_ms = Params::LOG_MS_FIRE;
    vTaskDelay(pdMS_TO_TICKS(50));
    if (m_devices.m_igniter.sendIgnition(recFirePassword)) {
      m_devices.m_indicators.showSuccess();
    }
    recFirePassword = "";  // clear the password
  }

  vTaskDelay(pdMS_TO_TICKS(1000));  //
}

void State_Machine::indicationTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));
  // Convert generic pointer back to State_Machine*
  auto *machine = static_cast<State_Machine *>(pvParameters);

  while (true) {
    bool requirementsMet = machine->m_devices.checkStatus();

    if (!requirementsMet) {
      machine->setCurrentState(CRITICAL_ERROR);
    }

    vTaskDelay(pdMS_TO_TICKS(Params::INDICATION_MS));
  }
}

void State_Machine::idleTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));
  // Convert generic pointer back to State_Machine*
  auto *machine = static_cast<State_Machine *>(pvParameters);

  while (true) {
    machine->m_commander.run();
    vTaskDelay(pdMS_TO_TICKS(Params::IDLE_MS));
  }
}

void State_Machine::LoRaTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));
  // Convert generic pointer back to State_Machine*
  auto *machine = static_cast<State_Machine *>(pvParameters);

  while (true) {
    String message =
        "State = " + machine->stateToString(machine->getCurrentState()) + "\n" +
        "Force = " + String(machine->m_filteredForce) + "N";
    machine->m_devices.m_LoRaCom.sendMessage(message.c_str());
    vTaskDelay(pdMS_TO_TICKS(machine->m_loraPeriod_ms));
  }
}

void State_Machine::logTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(50));
  // Convert generic pointer back to State_Machine*
  auto *machine = static_cast<State_Machine *>(pvParameters);

  ESP_LOGI(TAG, "Starting log Task");

  // !!! remove comments

  // bool successLog = machine->m_devices.m_logger.startNewLog();

  // if (successLog)
  // {
  //     // This method is more precise than vTaskDelay
  //     TickType_t xLastWakeTime = xTaskGetTickCount();              // Get
  //     initial tick count const TickType_t xFrequency =
  //     pdMS_TO_TICKS(Params::LOG_MS); // Logging period

  float DataList[] = {0.0f, 0.0f};  // fake data for testing

  unsigned long lastDisplayed = 0;  // last time the force was displayed

  while (true) {
    DataList[0] = static_cast<float>(machine->getCurrentState());
    DataList[1] = machine->m_filteredForce;
    // machine->m_devices.m_loadCell.getForceSample();
    machine->m_devices.m_logger.logData(DataList, Params::LOG_COLUMNS);

    if ((millis() - lastDisplayed) > Params::INDICATION_MS) {
      machine->m_devices.UI.updateForce(DataList[1]);
      lastDisplayed = millis();
    }

    vTaskDelay(pdMS_TO_TICKS(machine->m_logPeriod_ms));
  }
  // }
  // else
  // {
  //     machine->m_logTaskHandle = NULL; // clear the handle
  //     vTaskDelete(NULL);
  // }
}

void State_Machine::criticalErrorSeq() {
  ESP_LOGE("State_Machine CRITICAL ERROR", "Critical Error Sequence!");

  // m_devices.UI.showError("Critical Error");

  vTaskDelay(pdMS_TO_TICKS(2000));  // wait for 2 seconds before restarting
                                    // the initialisation sequence

  // destroy task
  vTaskDelete(m_indicationLoopTaskHandle);
  m_indicationLoopTaskHandle = NULL;  // clear the handle

  setCurrentState(INITIALISATION);
}

void State_Machine::lightSleepSeq() {
  if (m_logTaskHandle != NULL) {  // stop the log task before sleeping

    // flush the buffer first
    // m_devices.m_logger.forceFlush(); // !!! remove comment

    vTaskDelete(m_logTaskHandle);
    m_logTaskHandle = NULL;

    vTaskDelete(m_indicationLoopTaskHandle);
    m_indicationLoopTaskHandle = NULL;
  }

  if (m_devices.sleepMode()) {
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "Entering Light Sleep!");
    esp_light_sleep_start();  // Enter Light Sleep

    ESP_LOGI(TAG, "Waking up from light sleep!");
    m_devices.wakeMode();
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  setCurrentState(IDLE);
}

void State_Machine::setupCommands() {
  m_commander.addCommand('A', [this](const char *arg) {
    ESP_LOGI(TAG, "Command A received: %s", arg);
    m_devices.m_indicators.showSuccess();
  });

  m_commander.addCommand('F', [this](const char *arg) {
    ESP_LOGI(TAG, "Command [FIRE], Password [%s]", arg);
    if (m_devices.m_igniter.igniterReady()) {
      setCurrentState(FIRE);
      recFirePassword = const_cast<char *>(arg);
      // give some time to get into the fire state
      return;
    }
    ESP_LOGI(TAG, "Could not fire!");
  });

  m_commander.addCommand('R', [this](const char *arg) {
    ESP_LOGI(TAG, "Command R received: %s", arg);
    int16_t rssi = m_devices.m_LoRaCom.getRssi();
    String msg = "UPLINK_RSSI = " + String(m_devices.m_LoRaCom.getRssi());
    m_devices.m_LoRaCom.sendMessage(msg.c_str());
  });
}

void State_Machine::initialisationSeq() {
  m_commander.init(&m_devices.m_serialCom, &m_devices.m_LoRaCom);
  setupCommands();

  STATES currState = m_devices.begin() ? CALIBRATION : CRITICAL_ERROR;

  // if (currState == CALIBRATION) {
  //   setupHardwareTimer(2000, State_Machine::onTimerInterrupt);
  // }

  setCurrentState(currState);

  indicationSeq();

  m_logPeriod_ms = Params::LOG_MS_IDLE;  // set the log period to idle
  m_loraPeriod_ms = 1000.0f;             // 1Hz until fire sequence starts
}

void State_Machine::indicationSeq() {
  if (m_indicationLoopTaskHandle == NULL) {
    xTaskCreate(&State_Machine::indicationTask, "Indication Loop Task", 8192,
                this, PRIORITY_LOW,
                &m_indicationLoopTaskHandle);  // uses 1836 bytes of stack
  }
}

bool State_Machine::calibrationSeq() {
  vTaskDelay(pdMS_TO_TICKS(50));

  ESP_LOGI("State_Machine CALIBRATION", "Calibration Sequence!");

  bool succ = m_devices.calibrate();

  if (succ) {
    m_devices.m_indicators.showSuccess();
    // m_devices.UI.showSuccess("Calibration");
  }

  setCurrentState(
      (succ) ? IDLE
             : IDLE);  // !!!CRITICAL_ERROR, instead of critical error, either
  // make new state or just go to idle and wait for button
  // press to re-calibrate !!!!!!!!!!!!!!!!!!!!!!

  return succ;
}

void State_Machine::logSeq() {
  if (m_logTaskHandle == NULL) {
    xTaskCreate(&State_Machine::logTask, "Starting log Task", 4096, this,
                PRIORITY_HIGH, &m_logTaskHandle);
  }
}

void State_Machine::loRaSeq() {
  if (m_LoRaTaskHandle == NULL) {
    xTaskCreate(&State_Machine::LoRaTask, "Starting LoRa Task", 4096, this,
                PRIORITY_LOW, &m_LoRaTaskHandle);
  }
}

void State_Machine::idleSeq() {
  if (m_idleTaskHandle == NULL) {
    ESP_LOGI(TAG, "Idle Sequence!");
    xTaskCreate(&State_Machine::idleTask, "Starting idle Task", 4096, this,
                PRIORITY_LOW, &m_idleTaskHandle);
  }
}

STATES State_Machine::getCurrentState() {
  STATES currState;

  {
    SemaphoreGuard guard(m_stateMutex);
    if (guard.acquired()) {
      currState = m_currState;
    }
  }

  return currState;
}

void State_Machine::setCurrentState(STATES state) {
  SemaphoreGuard guard(m_stateMutex);
  if (guard.acquired()) {
    m_currState = state;
  }
}

String State_Machine::stateToString(STATES state) {
  switch (state) {
    case INITIALISATION:
      return "INITIALISATION";
    case CRITICAL_ERROR:
      return "CRITICAL_ERROR";
    case CALIBRATION:
      return "CALIBRATION";
    case IDLE:
      return "IDLE";
    case LIGHT_SLEEP:
      return "LIGHT_SLEEP";
    case FIRE:
      return "FIRE";
    default:
      return "UNKNOWN_STATE";
  }
}

// if (machine->m_devices.m_button.isPressed()) {
//   machine->m_devices.m_igniter.sendIgnition("12345");
// }