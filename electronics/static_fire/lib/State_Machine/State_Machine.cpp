#include "State_Machine.hpp"
#include "Params.hpp"
// #include "perfmon.hpp"

State_Machine::State_Machine() : m_control(m_devices)
{
    m_stateMutex = xSemaphoreCreateMutex();
}

State_Machine::~State_Machine()
{
}

void State_Machine::begin()
{
    // perfmon_start();

    {
        SemaphoreGuard guard(m_stateMutex);
        if (guard.acquired())
        {
            m_currState = INITIALISATION;
        }
    }

    if (m_taskManagerTaskHandle == NULL)
    {
        ESP_LOGI(TAG, "Starting Task Manager Task");
        xTaskCreate(&State_Machine::taskManagerTask, "Starting Task Manager", 4096, this, PRIORITY_MEDIUM, &m_taskManagerTaskHandle);
    }
}

void State_Machine::taskManagerTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    while (true)
    {
        machine->loop();

        // ESP_LOGI(TAG, "Current state: %s", machine->stateToString(static_cast<STATES>(machine->getCurrentState())));
        // UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        // ESP_LOGI(TAG, "Remaining stack: %u words\n", uxHighWaterMark);

        vTaskDelay(pdMS_TO_TICKS(Params::TASK_MANAGER_MS)); // Loop current has blocking in certain functions
    }
}

void State_Machine::loop()
{
    STATES currState;

    {
        SemaphoreGuard guard(m_stateMutex);
        if (guard.acquired())
        {
            currState = m_currState;
        }
    }

    switch (currState)
    {
    case INITIALISATION:
    {
        initialisationSeq();
    }
    break;
    case CRITICAL_ERROR:
    {
        criticalErrorSeq();
    }
    break;
    case CALIBRATION:
    {
        calibrationSeq();
        logSeq();  // initial logging start
        wifiSeq(); // start wifi server
    }
    break;
    case IDLE:
    {
        idleSeq(); // this is a blocking function. fine for our case but not good for task manager
    }
    break;
    case LIGHT_SLEEP:
    {
        lightSleepSeq();
        indicationSeq();
        logSeq();  // go back to logging after sleep
        wifiSeq(); // re-start wifi server
    }
    break;
    case CONTROL:
    {
        controlSeq();
    }
    break;
    default:
        break;
    }
}

void State_Machine::BLDCTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    machine->m_devices.m_bldc.enableMotor(true); // Enable motor

    TickType_t xLastWakeTime = xTaskGetTickCount();               // Store initial tick count
    const TickType_t xFrequency = pdMS_TO_TICKS(Params::BLDC_MS); // Task period

    while (machine->m_control.getControllable())
    {
        {
            // TimerGuard guard(TAG, "update BLDC task");
            machine->m_control.updateBLDC(); // currently only takes ~ 5uS or 200kHz
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

    machine->m_devices.m_bldc.enableMotor(false); // Disable motor

    // go to idle state
    {
        SemaphoreGuard guard(machine->m_stateMutex);
        if (guard.acquired())
        {
            machine->m_currState = IDLE;
        }
    }

    ESP_LOGI(TAG, "Ending BLDC Task!");

    // Delete the task explicitly
    machine->m_BLDCTaskHandle = NULL;
    vTaskDelete(NULL);
}

void State_Machine::balanceTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    machine->m_control.setState();

    while (true)
    {
        machine->m_control.updateBalanceControl(Params::BALANCE_MS); // currently only takes ~ 30uS or 33kHz
        vTaskDelay(pdMS_TO_TICKS(Params::BALANCE_MS));
    }
}

void State_Machine::updateFiltersTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    while (true)
    {
        {
            // TimerGuard guard(TAG, "update filter task");
            machine->m_control.updateData(); // takes ~ 380uS or 2.63kHz
        }
        vTaskDelay(pdMS_TO_TICKS(Params::AQUISITION_MS));
    }
}

void State_Machine::indicationTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    while (true)
    {
        bool requirementsMet = machine->m_devices.indicateStatus();

        if (!requirementsMet)
        {
            {
                SemaphoreGuard guard(machine->m_stateMutex);
                if (guard.acquired())
                {
                    machine->m_currState = CRITICAL_ERROR;
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(Params::INDICATION_MS));
    }
}

void State_Machine::refreshStatusTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(Params::REFRESH_STATUS_MS)); // delay first to create initial offset
        machine->m_devices.refreshStatusAll();
    }
}

void State_Machine::logTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(50));
    // Convert generic pointer back to State_Machine*
    auto *machine = static_cast<State_Machine *>(pvParameters);

    ESP_LOGI(TAG, "Starting log Task");

    bool successLog = machine->m_devices.m_logger.startNewLog();

    if (successLog)
    {
        // This method is more precise than vTaskDelay
        TickType_t xLastWakeTime = xTaskGetTickCount();              // Get initial tick count
        const TickType_t xFrequency = pdMS_TO_TICKS(Params::LOG_MS); // Logging period

        while (true)
        {
            machine->m_devices.m_logger.logData(machine->m_control.getDataBuffer(), Params::LOG_COLUMNS); // around 160 microseconds without flush (3 float points and time), ~37ms for flush (4kB)
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }
    else
    {
        machine->m_logTaskHandle = NULL; // clear the handle
        vTaskDelete(NULL);
    }
}

void State_Machine::criticalErrorSeq()
{
    ESP_LOGE("State_Machine CRITICAL ERROR", "Critical Error Sequence!");

    // destroy task
    vTaskDelete(m_refreshStatusTaskHandle);
    m_refreshStatusTaskHandle = NULL; // clear the handle

    vTaskDelay(pdMS_TO_TICKS(5000)); // wait for 5 seconds before restarting the initialisation sequence

    // destroy task
    vTaskDelete(m_indicationLoopTaskHandle);
    m_indicationLoopTaskHandle = NULL; // clear the handle

    {
        SemaphoreGuard guard(m_stateMutex);
        if (guard.acquired())
        {
            m_currState = INITIALISATION;
        }
    }
}

void State_Machine::lightSleepSeq()
{
    if (m_logTaskHandle != NULL)
    { // stop the log task before sleeping

        // flush the buffer first
        m_devices.m_logger.forceFlush();

        vTaskDelete(m_logTaskHandle);
        m_logTaskHandle = NULL;

        vTaskDelete(m_indicationLoopTaskHandle);
        m_indicationLoopTaskHandle = NULL;
    }

    if (m_devices.sleepMode())
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        ESP_LOGI(TAG, "Entering Light Sleep!");
        esp_light_sleep_start(); // Enter Light Sleep

        ESP_LOGI(TAG, "Waking up from light sleep!");
        m_devices.wakeMode();
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    SemaphoreGuard guard(m_stateMutex);
    if (guard.acquired())
    {
        m_currState = IDLE;
    }
}

void State_Machine::initialisationSeq()
{
    STATES currState = m_devices.init(Params::LOG_SD, Params::LOG_SERIAL, Params::SILENT_INDICATION, Params::SERVO_BRAKING, Params::USE_IMU, Params::USE_ROT_ENC) ? CALIBRATION : CRITICAL_ERROR;

    {
        SemaphoreGuard guard(m_stateMutex);
        if (guard.acquired())
        {
            m_currState = currState;
        }
    }

    indicationSeq();
}

void State_Machine::indicationSeq()
{

    if (m_indicationLoopTaskHandle == NULL)
    {
        xTaskCreate(&State_Machine::indicationTask, "Indication Loop Task", 2048, this, PRIORITY_LOW, &m_indicationLoopTaskHandle); // uses 1836 bytes of stack
    }
    if (m_refreshStatusTaskHandle == NULL)
    {
        xTaskCreate(&State_Machine::refreshStatusTask, "Device status check loop Task", 2048, this, PRIORITY_LOW, &m_refreshStatusTaskHandle);
    }
}

void State_Machine::calibrationSeq()
{
    vTaskDelay(pdMS_TO_TICKS(50));

    ESP_LOGI("State_Machine CALIBRATION", "Calibration Sequence!");

    bool succ = m_devices.calibrateSeq();

    succ &= m_control.setup();

    if ((m_updateFiltersTaskHandle == NULL) && succ)
    {
        xTaskCreate(&State_Machine::updateFiltersTask, "Starting Filters Task", 4096, this, PRIORITY_HIGH, &m_updateFiltersTaskHandle);
    }

    SemaphoreGuard guard(m_stateMutex);
    if (guard.acquired())
    {
        m_currState = (succ) ? IDLE : CRITICAL_ERROR;
    }

    if (succ)
    {
        m_devices.m_indicators.showSuccess();
    }
}

void State_Machine::wifiSeq()
{
    xTaskCreatePinnedToCore(&State_Machine::wifiTask, "WiFi Task", 4096, this, PRIORITY_HIGH, &m_wifiTaskHandle, 0); // core 0 for wifi tasks
}

void State_Machine::controlSeq()
{
    // ESP_LOGI(TAG, "Control Sequence!");

    if (m_balanceTaskHandle == NULL)
    {
        ESP_LOGI(TAG, "Starting balance Task");
        xTaskCreate(&State_Machine::balanceTask, "balance Task", 4096, this, PRIORITY_HIGH, &m_balanceTaskHandle);
    }

    if (m_BLDCTaskHandle == NULL)
    {
        ESP_LOGI(TAG, "Starting BLDC Task");
        xTaskCreate(&State_Machine::BLDCTask, "Starting BLDC Task", 4096, this, PRIORITY_HIGH, &m_BLDCTaskHandle);
    }
}

void State_Machine::logSeq()
{
    if (m_logTaskHandle == NULL)
    {
        xTaskCreate(&State_Machine::logTask, "Starting log Task", 4096, this, PRIORITY_MEDIUM, &m_logTaskHandle);
    }
}

STATES State_Machine::getCurrentState()
{
    STATES currState;

    {
        SemaphoreGuard guard(m_stateMutex);
        if (guard.acquired())
        {
            currState = m_currState;
        }
    }

    return currState;
}

void State_Machine::idleSeq()
{
    ESP_LOGI(TAG, "Idle Sequence!");
    vTaskDelay(pdMS_TO_TICKS(50));
}

const char *State_Machine::stateToString(STATES state)
{
    switch (state)
    {
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
    case CONTROL:
        return "CONTROL";
    default:
        return "UNKNOWN_STATE";
    }
}