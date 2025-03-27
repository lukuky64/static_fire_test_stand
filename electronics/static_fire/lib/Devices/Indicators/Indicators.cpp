#include "Indicators.hpp"

Indicators::Indicators()
{
    m_ledMutex = nullptr; // Initialize the mutex to nullptr
}

Indicators::~Indicators()
{
    if (m_ledMutex != nullptr)
    {
        vSemaphoreDelete(m_ledMutex);
    }
}

/*****************************************************************************/
/*!
    @brief Setup the  LED and create the LED mutex. Flash the LED
    to indicate success.

    @param ledPin The GPIO pin to be used for the LED.
*/
/*****************************************************************************/
void Indicators::setup(uint8_t ledPin)
{
    m_ledPin = ledPin;
    pinMode(m_ledPin, OUTPUT);

    m_ledMutex = xSemaphoreCreateMutex(); // Create the RGB LED mutex

    // quick test
    controlLed(50);
}

/*****************************************************************************/
/*!
    @brief Show a critical error indication. This will play a tone and flash red.
*/
/*****************************************************************************/
void Indicators::showCriticalError()
{
    ESP_LOGI(TAG, "Showing Critical Error!");

    uint16_t duration = 50; // ms

    for (int i = 0; i < 3; i++)
    {
        controlLed(duration);                // Red
        vTaskDelay(pdMS_TO_TICKS(duration)); // off period
    }
}

/*****************************************************************************/
/*!
    @brief Show a warning indication. This will flash orange two times.
*/
/*****************************************************************************/
void Indicators::showWarning()
{
    ESP_LOGI(TAG, "Showing Warning!");

    uint16_t duration = 50; // ms

    for (int i = 0; i < 2; i++)
    {
        controlLed(duration);                // Orange
        vTaskDelay(pdMS_TO_TICKS(duration)); // off period
    }
}

/*****************************************************************************/
/*!
    @brief Show a success indication. This will play a tone and flash green
    three times.
*/
/*****************************************************************************/
void Indicators::showSuccess()
{
    ESP_LOGI(TAG, "Showing Success!");

    uint16_t duration = 50; // ms

    for (int i = 0; i < 3; i++)
    {
        controlLed(duration); // Green
        vTaskDelay(pdMS_TO_TICKS(duration));
    }
}

/*****************************************************************************/
/*!
    @brief Show an all good indication. This will flash green once.
*/
/*****************************************************************************/
void Indicators::showAllGood()
{
    // ESP_LOGI(TAG, "Showing All Good!");

    uint16_t duration = 50; // ms

    controlLed(duration);
}

/*****************************************************************************/
/*!
    @brief Turn off all indicators. This will turn off the buzzer and RGB LED.
*/
/*****************************************************************************/
void Indicators::showAllOff()
{
    ESP_LOGI(TAG, "Turning off all Indicators!");

    controlLed(0);
}

/*****************************************************************************/
/*!
    @brief Control the RGB LED. This will flash the LED a given colour for
    a given duration.

    @param hexValue The hex value of the colour to be displayed (0xRRGGBB)

    @param duration The duration of the colour to be displayed (ms)

    @return True if the RGB LED was setup successfully, false otherwise.
*/
/*****************************************************************************/
bool Indicators::controlLed(int duration)
{
    SemaphoreGuard guard(m_ledMutex);
    if (guard.acquired())
    {
        digitalWrite(m_ledPin, HIGH);
        vTaskDelay(pdMS_TO_TICKS(duration));
        digitalWrite(m_ledPin, LOW);

        return true;
    }
    return false;
}