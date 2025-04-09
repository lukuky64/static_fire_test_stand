#include <params.hpp>

namespace Params {
// Global parameter declarations (means defined elsewhere)
unsigned int ALLOW_SLEEP = 0;
float SLP_TIMEOUT_MS = 10000;
float AQUISITION_FREQ = 10;
float LOG_FREQ = 1000;
float INDICATION_FREQ = 5;
float TASK_MANGR_FREQ = 5;
float IDLE_FREQ = 5;

unsigned int LOG_SD = 1;
unsigned int LOG_SERIAL = 0;

const char *LOG_FILE_PREFIX = "/LOG";
unsigned int LOG_STATE = 1;
unsigned int LOG_LOAD = 1;

unsigned int LOG_COLUMNS = 3;

// Periods (in ms)
float AQUISITION_MS = 1000 / AQUISITION_FREQ;
float LOG_MS = 1000 / LOG_FREQ;
float REFRESH_STATUS_MS = 10000;
float INDICATION_MS = 1000 / INDICATION_FREQ;
float TASK_MANAGER_MS = 1000 / TASK_MANGR_FREQ;
float IDLE_MS = 1000 / IDLE_FREQ;
}  // namespace Params
