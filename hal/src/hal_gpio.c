/**
 * @file hal_gpio.c
 * @brief GPIO wrapper over ST HAL GPIO (NUCLEO-L552ZE-Q LEDs + user button).
 *
 * Mirrors FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_gpio.c (table-driven,
 * status-returning, public API free of vendor types).
 */
#include "hal_gpio.h"
#include "stm32l5xx_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint8_t       is_output;
} hal_gpio_map_t;

static const hal_gpio_map_t s_map[HAL_GPIO_PIN_COUNT] = {
    [HAL_GPIO_LED_GREEN] = { GPIOC, GPIO_PIN_7,  1U },
    [HAL_GPIO_LED_BLUE]  = { GPIOB, GPIO_PIN_7,  1U },
    [HAL_GPIO_LED_RED]   = { GPIOG, GPIO_PIN_2,  1U },
    [HAL_GPIO_BTN_USER]  = { GPIOC, GPIO_PIN_13, 0U },
};

static uint8_t s_inited;

hal_err_t hal_gpio_init(void)
{
    if (0U != s_inited) {
        return HAL_ERR_SUCCESS;
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();          /* PG[15:2] rail */

    for (uint32_t i = 0U; i < (uint32_t)HAL_GPIO_PIN_COUNT; i++) {
        GPIO_InitTypeDef gi = {0};
        gi.Pin   = s_map[i].pin;
        gi.Pull  = GPIO_NOPULL;
        gi.Speed = GPIO_SPEED_FREQ_LOW;
        gi.Mode  = s_map[i].is_output ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        if (s_map[i].is_output) {
            HAL_GPIO_WritePin(s_map[i].port, s_map[i].pin, GPIO_PIN_RESET);
        }
        HAL_GPIO_Init(s_map[i].port, &gi);
    }

    s_inited = 1U;
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_level_t level)
{
    if ((uint32_t)pin >= (uint32_t)HAL_GPIO_PIN_COUNT) {
        return HAL_ERR_GPIO_INVALID_PIN;
    }
    HAL_GPIO_WritePin(s_map[pin].port, s_map[pin].pin,
                      (HAL_GPIO_LEVEL_HIGH == level) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_gpio_read(hal_gpio_pin_t pin, hal_gpio_level_t *level_out)
{
    if ((uint32_t)pin >= (uint32_t)HAL_GPIO_PIN_COUNT || level_out == NULL) {
        return HAL_ERR_GPIO_INVALID_PIN;
    }
    *level_out = (GPIO_PIN_SET == HAL_GPIO_ReadPin(s_map[pin].port, s_map[pin].pin))
                 ? HAL_GPIO_LEVEL_HIGH : HAL_GPIO_LEVEL_LOW;
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_gpio_toggle(hal_gpio_pin_t pin)
{
    if ((uint32_t)pin >= (uint32_t)HAL_GPIO_PIN_COUNT) {
        return HAL_ERR_GPIO_INVALID_PIN;
    }
    HAL_GPIO_TogglePin(s_map[pin].port, s_map[pin].pin);
    return HAL_ERR_SUCCESS;
}

#if defined(HAL_UNIT_TEST) && HAL_UNIT_TEST
void hal_gpio_unit_test(void)
{
    (void)hal_gpio_init();
    (void)hal_gpio_toggle(HAL_GPIO_LED_GREEN);
}
#endif
