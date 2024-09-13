#ifndef HAL_UTILS_H
#define HAL_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "hal_config.h"
/**
 * @brief Initializes the system for microsecond delays.
 *
 * This function enables the Data Watchpoint and Trace (DWT) cycle counter
 * for use in accurate microsecond delays.
 */
void dwt_init(void);

/**
 * @brief Delays execution for a specified number of microseconds.
 *
 * This function uses the Data Watchpoint and Trace (DWT) cycle counter
 * for precise microsecond-level delays. It blocks the CPU during this time.
 *
 * @param us The number of microseconds to delay.
 */
void hal_delay_us(size_t us);

/**
 * @brief Delays execution for a specified number of milliseconds.
 *
 * This function uses the HAL_Delay function from STM32 HAL to provide
 * millisecond-level delays. It blocks the CPU during this time.
 *
 * @param ms The number of milliseconds to delay.
 */
void hal_delay_ms(size_t ms);

/**
 * @brief Puts the system into sleep mode for a specified number of microseconds.
 *
 * This function approximates a microsecond sleep by using a delay. It does not
 * actually put the system into a low-power sleep state.
 *
 * @param us The number of microseconds to sleep.
 */
void hal_sleep_us(size_t us);

/**
 * @brief Puts the system into sleep mode for a specified number of milliseconds.
 *
 * This function puts the STM32 into a low-power sleep mode for a specified
 * number of milliseconds. It uses WFI (Wait For Interrupt) to wait.
 *
 * @param ms The number of milliseconds to sleep.
 */

#endif // HAL_UTILS_H