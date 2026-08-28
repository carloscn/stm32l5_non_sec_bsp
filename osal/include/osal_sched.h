/**
 * @file osal_sched.h
 * @brief Scheduler state + critical sections.
 */
#ifndef OSAL_SCHED_H
#define OSAL_SCHED_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @return true if FreeRTOS scheduler is running. */
bool osal_sched_is_running(void);

void osal_sched_critical_enter(void);
void osal_sched_critical_exit(void);

/** Call at end of ISR when a FromISR API set higher_prio_woken. */
void osal_sched_yield_from_isr(bool higher_prio_woken);

/** Start the FreeRTOS scheduler (does not return on success). */
void osal_sched_start(void);

/** Milliseconds since scheduler start (wraps). Before start: 0. */
uint32_t osal_time_get_ms(void);

/** RTOS kernel version string, e.g. "V11.1.0". */
const char *osal_sched_kernel_version(void);

/**
 * Unrecoverable error: log @p reason (best effort), disable interrupts, halt.
 * Does not return. Safe to call before osal_log_init() / before the scheduler.
 */
void osal_panic(const char *reason) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

#endif /* OSAL_SCHED_H */
