/**
 * @file led.c
 * @brief LED blink task. Platform access via hal_gpio / osal only.
 */
#include "led.h"
#include "hal_gpio.h"
#include "osal_task.h"
#include "osal_log.h"

#define LED_PIN         HAL_GPIO_LED_GREEN
#define LED_PERIOD_MS   (250U)
#define LED_STACK_WORDS (128U)
#define LED_PRIORITY    (2U)

static void led_task(void *arg)
{
    (void)arg;
    for (;;) {
        hal_gpio_toggle(LED_PIN);
        osal_task_delay_ms(LED_PERIOD_MS);
    }
}

void led_start(void)
{
    if (osal_task_create(led_task, "led", LED_STACK_WORDS, NULL,
                         LED_PRIORITY, NULL) != OSAL_OK) {
        osal_log_info("led: task create failed");
    }
}
