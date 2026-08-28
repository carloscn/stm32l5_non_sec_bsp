/**
 * @file hal_mcu.h
 * @brief MCU-level HAL (reset / chip control — not peripherals).
 *
 * API kept identical to FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_mcu.h.
 */
#ifndef HAL_MCU_H
#define HAL_MCU_H

#include <stdint.h>
#include "hal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bring up the MCU: core HAL init, clock tree, flash latency, caches.
 *        Everything platform-specific that must run before peripherals.
 *        Call once, first thing in main().
 * @return HAL_ERR_SUCCESS or a HAL_ERR_* code.
 */
hal_err_t hal_mcu_init(void);

/**
 * @brief Request a full chip reset (Cortex-M33 SYSRESETREQ via SCB->AIRCR).
 * @note Does not return on success. Call only after any in-flight response
 *       (e.g. a UDS positive response) has been flushed.
 */
void hal_mcu_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_MCU_H */
