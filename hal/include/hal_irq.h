/**
 * @file hal_irq.h
 * @brief NVIC / interrupt-controller HAL (no vendor types in the public header).
 *
 * Same API as FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_irq.h.
 * STM32L552 (Cortex-M33) implementation is a thin wrapper over CMSIS NVIC_*.
 * Handler installation targets a RAM vector table (see hal_irq.c).
 */
#ifndef HAL_IRQ_H
#define HAL_IRQ_H

#include <stdint.h>
#include "hal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Default peripheral ISR priority. Must be numerically >= (i.e. lower urgency
 * than) configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY so the ISR may call
 * FreeRTOS *FromISR APIs.
 */
#define HAL_IRQ_PRIO_DEFAULT  (5U)

typedef void (*hal_irq_handler_t)(void);

/**
 * Relocate the vector table to RAM (so hal_irq_install_handler works) and
 * apply default priorities. Idempotent.
 */
hal_err_t hal_irq_init(void);

hal_err_t hal_irq_enable(uint8_t irq);
hal_err_t hal_irq_disable(uint8_t irq);
hal_err_t hal_irq_set_priority(uint8_t irq, uint8_t priority);
hal_err_t hal_irq_install_handler(uint8_t irq, hal_irq_handler_t handler);

void hal_irq_global_enable(void);
void hal_irq_global_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_IRQ_H */
