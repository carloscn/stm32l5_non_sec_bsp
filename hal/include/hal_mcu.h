/**
 * @file hal_mcu.h
 * @brief MCU-level HAL (reset / chip control — not peripherals).
 *
 * API kept identical to FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_mcu.h.
 */
#ifndef HAL_MCU_H
#define HAL_MCU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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
