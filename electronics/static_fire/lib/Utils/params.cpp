#include <params.hpp>

namespace Params
{
        // Global parameter declarations (means defined elsewhere)
        unsigned int ALLOW_SLEEP = 0;
        float SLP_TIMEOUT_MS = 10000;
        float AQUISITION_FREQ = 10;
        float LOG_FREQ = 10;
        float INDICATION_FREQ = 5;
        float TASK_MANGR_FREQ = 5;

        unsigned int LOG_SD = 1;
        unsigned int LOG_SERIAL = 0;
        unsigned int SILENT_INDICATION = 0;

        const char *LOG_FILE_PREFIX = "/LOG";
        unsigned int LOG_THETA = 0;
        unsigned int LOG_THETA_DOT = 0;
        unsigned int LOG_PHI = 0;
        unsigned int LOG_PHI_DOT = 0;
        unsigned int LOG_SETPOINT = 0;

        unsigned int LOG_COLUMNS = 3;

        // Periods (in ms)
        float AQUISITION_MS = 1000 / AQUISITION_FREQ;
        float LOG_MS = 1000 / LOG_FREQ;
        float REFRESH_STATUS_MS = 10000;
        float INDICATION_MS = 1000 / INDICATION_FREQ;
        float TASK_MANAGER_MS = 1000 / TASK_MANGR_FREQ;
}
