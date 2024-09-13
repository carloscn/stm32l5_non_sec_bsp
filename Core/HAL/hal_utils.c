#include "stm32l5xx.h"
#include "stm32l5xx_hal.h"
#include "hal_utils.h"
#include "hal_err.h"

// Delay for a specified number of microseconds using DWT
void hal_delay_us(size_t us)
{

}

// Delay for a specified number of milliseconds (uses HAL_Delay)
void hal_delay_ms(size_t ms)
{
    HAL_Delay(ms);  // Use STM32 HAL's built-in millisecond delay
}

// Sleep for a specified number of microseconds (approximate, uses busy wait)
void hal_sleep_us(size_t us)
{
    hal_delay_us(us);  // Approximate microsecond sleep using delay
}

// Sleep for a specified number of milliseconds
void hal_sleep_ms(size_t ms)
{
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);  // Sleep mode
    hal_delay_ms(ms);  // Add delay after wakeup
}