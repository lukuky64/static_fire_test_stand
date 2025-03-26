#pragma once

#include <Arduino.h>

#define MAX_LOG_COLUMNS 10

// Dummy device selection
#define DUMMY_SD 0

namespace Params
{
        // Global parameter declarations (means defined elsewhere)
        extern unsigned int ALLOW_SLEEP;
        extern float SLP_TIMEOUT_MS;
        extern float AQUISITION_FREQ;
        extern float LOG_FREQ;
        extern float INDICATION_FREQ;
        extern float TASK_MANGR_FREQ;

        extern unsigned int LOG_SD;
        extern unsigned int LOG_SERIAL;
        extern unsigned int SILENT_INDICATION;

        extern const char *LOG_FILE_PREFIX;
        extern unsigned int LOG_THETA;
        extern unsigned int LOG_THETA_DOT;
        extern unsigned int LOG_PHI;
        extern unsigned int LOG_PHI_DOT;
        extern unsigned int LOG_SETPOINT;

        extern unsigned int LOG_COLUMNS;

        // Periods (in ms)
        extern float AQUISITION_MS;
        extern float LOG_MS;
        extern float REFRESH_STATUS_MS;
        extern float INDICATION_MS;
        extern float TASK_MANAGER_MS;
}
