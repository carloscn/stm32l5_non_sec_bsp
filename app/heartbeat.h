/**
 * @file heartbeat.h
 * @brief Heartbeat log task entry. Task creation is done in main().
 */
#ifndef APP_HEARTBEAT_H
#define APP_HEARTBEAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Suggested scheduling parameters for main() to pass to osal_task_create(). */
#define HEARTBEAT_TASK_STACK_WORDS   (256U)
#define HEARTBEAT_TASK_PRIORITY      (2U)

/** FreeRTOS/OSAL task function: logs a counter + free heap once per second. */
void heartbeat_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* APP_HEARTBEAT_H */
