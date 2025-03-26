#include "State_Machine.hpp"
// #include "perfmon.hpp"

State_Machine::State_Machine()
{
    m_stateMutex = xSemaphoreCreateMutex();
}

State_Machine::~State_Machine()
{
}

void State_Machine::begin()
{
    // perfmon_start();

    setCurrentState(INITIALISATION);

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
    STATES currState = getCurrentState();

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
        // !!! commenting out so i can just look at initialisaiton for now

        // calibrationSeq();
        // logSeq(); // initial logging start
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
        logSeq(); // go back to logging after sleep
    }
    break;
    case CONTROL:
    {
        //
    }
    break;
    default:
        break;
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
            machine->setCurrentState(CRITICAL_ERROR);
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

    // !!! remove comments

    // bool successLog = machine->m_devices.m_logger.startNewLog();

    // if (successLog)
    // {
    //     // This method is more precise than vTaskDelay
    //     TickType_t xLastWakeTime = xTaskGetTickCount();              // Get initial tick count
    //     const TickType_t xFrequency = pdMS_TO_TICKS(Params::LOG_MS); // Logging period

    //     while (true)
    //     {

    //         const float fakeData[] = {1, 2, 3};                                 // !!! replace
    //         machine->m_devices.m_logger.logData(fakeData, Params::LOG_COLUMNS); // around 160 microseconds without flush (3 float points and time), ~37ms for flush (4kB)
    //         vTaskDelayUntil(&xLastWakeTime, xFrequency);
    //     }
    // }
    // else
    // {
    //     machine->m_logTaskHandle = NULL; // clear the handle
    //     vTaskDelete(NULL);
    // }
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

    setCurrentState(INITIALISATION);
}

void State_Machine::lightSleepSeq()
{
    if (m_logTaskHandle != NULL)
    { // stop the log task before sleeping

        // flush the buffer first
        // m_devices.m_logger.forceFlush(); // !!! remove comment

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

    setCurrentState(IDLE);
}

void State_Machine::initialisationSeq()
{
    // STATES currState = m_devices.init(Params::LOG_SD, Params::LOG_SERIAL) ? CALIBRATION : CRITICAL_ERROR;

    STATES currState = m_devices.begin() ? CALIBRATION : CRITICAL_ERROR;

    setCurrentState(currState);

    indicationSeq();
}

void State_Machine::indicationSeq()
{

    // if (m_indicationLoopTaskHandle == NULL)
    // {
    //     xTaskCreate(&State_Machine::indicationTask, "Indication Loop Task", 2048, this, PRIORITY_LOW, &m_indicationLoopTaskHandle); // uses 1836 bytes of stack
    // }
    // if (m_refreshStatusTaskHandle == NULL)
    // {
    //     xTaskCreate(&State_Machine::refreshStatusTask, "Device status check loop Task", 2048, this, PRIORITY_LOW, &m_refreshStatusTaskHandle);
    // }
}

void State_Machine::calibrationSeq()
{
    vTaskDelay(pdMS_TO_TICKS(50));

    ESP_LOGI("State_Machine CALIBRATION", "Calibration Sequence!");

    bool succ = m_devices.calibrateSeq();

    if ((m_updateFiltersTaskHandle == NULL) && succ)
    {
        xTaskCreate(&State_Machine::updateFiltersTask, "Starting Filters Task", 4096, this, PRIORITY_HIGH, &m_updateFiltersTaskHandle);
    }

    setCurrentState((succ) ? IDLE : CRITICAL_ERROR);

    if (succ)
    {
        // m_devices.m_indicators.showSuccess(); // !!! remove comment
    }
}

void State_Machine::logSeq()
{
    if (m_logTaskHandle == NULL)
    {
        xTaskCreate(&State_Machine::logTask, "Starting log Task", 4096, this, PRIORITY_MEDIUM, &m_logTaskHandle);
    }
}

void State_Machine::idleSeq()
{
    ESP_LOGI(TAG, "Idle Sequence!");
    vTaskDelay(pdMS_TO_TICKS(50));
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

void State_Machine::setCurrentState(STATES state)
{
    SemaphoreGuard guard(m_stateMutex);
    if (guard.acquired())
    {
        m_currState = state;
    }
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