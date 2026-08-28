/**
 * @file led.c
 * @brief LED blink task body. No task creation here (see main()).
 */
#include "led.h"
#include "hal_gpio.h"
#include "osal_task.h"   /* osal_task_delay_ms */

#define LED_PIN         HAL_GPIO_LED_GREEN
#define LED_PERIOD_MS   (250U)

void led_task(void *arg)
{
    (void)arg;
    for (;;) {
        hal_gpio_toggle(LED_PIN);
        osal_task_delay_ms(LED_PERIOD_MS);
    }
}
