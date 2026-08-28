/**
 * @file heartbeat.h
 * @brief Periodic heartbeat / liveness log task (HAL/OSAL only).
 */
#ifndef APP_HEARTBEAT_H
#define APP_HEARTBEAT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the heartbeat task: logs a counter + free heap once per second.
 * Call before osal_sched_start().
 */
void heartbeat_start(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_HEARTBEAT_H */
