
#ifndef OSAL_LOG_H_
#define OSAL_LOG_H_

#include <stdint.h>
#include "hal_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_BUFFER_SIZE 256u

#ifndef OSAL_LOG_TIMESTAMP_ENABLE
#define OSAL_LOG_TIMESTAMP_ENABLE 1
#endif

/** Queue depth for deferred log lines (drop newest when full). */
#ifndef OSAL_LOG_QUEUE_LEN
#define OSAL_LOG_QUEUE_LEN 8u
#endif

/**
 * Create the log queue and LogTask. Call after UART init, before
 * vTaskStartScheduler(). Boot-time osal_log_info() before this still
 * falls back to blocking SyncSend.
 */
void osal_log_init(HAL_UART *uart);

/**
 * Enqueue a log line (non-blocking when scheduler is running).
 * Before the scheduler starts, transmits synchronously via UART.
 */
void osal_log_info(const char *msg);

/**
 * Formatted log (for configPRINTF). Same enqueue / SyncSend rules.
 */
void osal_log_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_LOG_H_ */
