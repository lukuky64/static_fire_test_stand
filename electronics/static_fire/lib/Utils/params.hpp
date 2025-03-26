#pragma once

#include <Arduino.h>

#define MAX_LOG_COLUMNS 10

// Dummy device selection
#define DUMMY_SD 0

namespace Params
{
        // Global parameter declarations (means defined elsewhere)
        unsigned int ALLOW_SLEEP = 0;
        float SLP_TIMEOUT_MS;
        float AQUISITION_FREQ;
        float BALANCE_FREQ;
        float BLDC_FREQ;
        float LOG_FREQ;
        float REFR_STAT_FREQ;
        float INDICATION_FREQ;
        float TASK_MANGR_FREQ;

        unsigned int LOG_SD;
        unsigned int LOG_SERIAL;
        unsigned int SILENT_INDICATION;
        unsigned int SERVO_BRAKING;
        unsigned int USE_ROT_ENC;
        unsigned int USE_IMU;

        const char *LOG_FILE_PREFIX;
        unsigned int LOG_THETA;
        unsigned int LOG_THETA_DOT;
        unsigned int LOG_PHI;
        unsigned int LOG_PHI_DOT;
        unsigned int LOG_SETPOINT;

        unsigned int NUM_POLES;
        float PHASE_RES;
        float SENSE_MVPA;

        float RATE_LIMIT;
        float MOTOR_KV;
        float ANGLE_THRESH;
        float LQR_K1;
        float LQR_K2;
        float LQR_K3;
        float LQR_K4;

        float JERK_KP;
        float JERK_KD;
        float WHEEL_J;
        float BALANCE_ANGLE;
        float BALANCE_PERIOD;

        unsigned int ENC_PPR;
        unsigned int LOG_COLUMNS;

        // Periods (in ms)
        float AQUISITION_MS;
        float BALANCE_MS;
        float BLDC_MS;
        float LOG_MS;
        float REFRESH_STATUS_MS;
        float INDICATION_MS;
        float TASK_MANAGER_MS;
}
