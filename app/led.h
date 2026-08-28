/**
 * @file led.h
 * @brief LED blink task entry. Task creation is done in main().
 */
#ifndef APP_LED_H
#define APP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Suggested scheduling parameters for main() to pass to osal_task_create(). */
#define LED_TASK_STACK_WORDS   (128U)
#define LED_TASK_PRIORITY      (2U)

/** FreeRTOS/OSAL task function: toggles the green user LED forever. */
void led_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* APP_LED_H */
