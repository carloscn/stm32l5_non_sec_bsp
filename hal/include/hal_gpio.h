/**
 * @file hal_gpio.h
 * @brief Board GPIO HAL (no ST LL/HAL types in the public API).
 *
 * API in sync with FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_gpio.h
 * (write/read/toggle return hal_err_t; read uses an out-parameter).
 * Only the pin enum is board-specific: NUCLEO-L552ZE-Q LEDs / button (UM2581).
 */
#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include "hal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HAL_GPIO_LED_GREEN = 0,   /* LD1  PC7  */
    HAL_GPIO_LED_BLUE,        /* LD2  PB7  */
    HAL_GPIO_LED_RED,         /* LD3  PA9  */
    HAL_GPIO_BTN_USER,        /* B1   PC13 (input) */
    HAL_GPIO_PIN_COUNT
} hal_gpio_pin_t;

typedef enum {
    HAL_GPIO_LEVEL_LOW  = 0,
    HAL_GPIO_LEVEL_HIGH = 1
} hal_gpio_level_t;

/** Configure the board pins above (clocks + mode). Idempotent. */
hal_err_t hal_gpio_init(void);

hal_err_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_level_t level);
hal_err_t hal_gpio_read(hal_gpio_pin_t pin, hal_gpio_level_t *level_out);
hal_err_t hal_gpio_toggle(hal_gpio_pin_t pin);

#if defined(HAL_UNIT_TEST) && HAL_UNIT_TEST
void hal_gpio_unit_test(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
