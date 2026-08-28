/**
 * @file led.c
 * @brief LED task body. No task creation here (see main()).
 *
 * NUCLEO-L552ZE-Q user LEDs (see CubeMX Core/Inc/main.h):
 *   LD1 green = PC7   (HAL_GPIO_LED_GREEN)
 *   LD2 blue  = PB7   (HAL_GPIO_LED_BLUE)
 *   LD3 red   = PA9   (HAL_GPIO_LED_RED)
 * Runs a 3-LED "chase" so a glance confirms the scheduler is alive.
 */
#include "led.h"
#include "hal_gpio.h"
#include "osal_task.h"   /* osal_task_delay_ms */

#define LED_STEP_MS   (150U)

static const hal_gpio_pin_t k_leds[] = {
    HAL_GPIO_LED_GREEN,
    HAL_GPIO_LED_BLUE,
    HAL_GPIO_LED_RED,
};
#define LED_COUNT  (sizeof(k_leds) / sizeof(k_leds[0]))

void led_task(void *arg)
{
    uint32_t i = 0U;
    (void)arg;

    for (;;) {
        for (uint32_t n = 0U; n < LED_COUNT; n++) {
            (void)hal_gpio_write(k_leds[n],
                                 (n == i) ? HAL_GPIO_LEVEL_HIGH : HAL_GPIO_LEVEL_LOW);
        }
        i = (i + 1U) % LED_COUNT;
        osal_task_delay_ms(LED_STEP_MS);
    }
}
