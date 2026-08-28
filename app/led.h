/**
 * @file led.h
 * @brief LED blink task (HAL/OSAL only).
 */
#ifndef APP_LED_H
#define APP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the LED task: toggles the green user LED at a fixed rate.
 * Call before osal_sched_start().
 */
void led_start(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_LED_H */
