#include "hal_err.h"
#include "hal_gpio.h"
#include "stm32l552xx.h"
#include "stm32l5xx_hal.h"
#include "stm32l5xx_hal_gpio.h"
#include <stdio.h>

static int32_t stm32l5xx_gpio_a9_init_map_to_gpio_0(void)
{
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    return HAL_SUCCESS;
}

static void stm32l5xx_gpio_a9_deinit_map_to_gpio_0(void)
{
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
}

static int32_t stm32l5xx_gpio_a9_read_pin_map_to_gpio_0(HAL_GPIO_PIN_STATE *res)
{
    int32_t ret = HAL_SUCCESS;
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);

    if (pin_state > GPIO_PIN_SET) {
        ret = HAL_ERR_UNSUPPORTED;
        goto finish;
    }

    *res = pin_state == GPIO_PIN_SET ? \
           HAL_GPIO_PIN_SET: \
           HAL_GPIO_PIN_RESET;
finish:
    return ret;
}

static void stm32l5xx_gpio_a9_write_pin_map_to_gpio_0(GPIO_PinState value)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, value);
}

static void stm32l5xx_gpio_a9_toggle_pin_map_to_gpio_0(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
}

static int32_t stm32l5xx_gpio_a9_lock_pin_map_to_gpio_0(void)
{
    int32_t ret = HAL_SUCCESS;
    HAL_StatusTypeDef ret_x = \
        HAL_GPIO_LockPin(GPIOA, GPIO_PIN_9);
    if (ret_x != HAL_OK) {
        ret = HAL_ERR_HW_CONTROL;
    }

    return ret;
}

static int32_t stm32l5xx_gpio_b7_init_map_to_gpio_1(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = 0;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    return HAL_SUCCESS;
}

static void stm32l5xx_gpio_b7_deinit_map_to_gpio_1(void)
{
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
}

static int32_t stm32l5xx_gpio_b7_read_pin_map_to_gpio_1(HAL_GPIO_PIN_STATE *res)
{
    int32_t ret = HAL_SUCCESS;
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);

    if (pin_state > GPIO_PIN_SET) {
        ret = HAL_ERR_UNSUPPORTED;
        goto finish;
    }

    *res = pin_state == GPIO_PIN_SET ? \
           HAL_GPIO_PIN_SET: \
           HAL_GPIO_PIN_RESET;
finish:
    return ret;
}

static void stm32l5xx_gpio_b7_write_pin_map_to_gpio_1(GPIO_PinState value)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, value);
}

static void stm32l5xx_gpio_b7_toggle_pin_map_to_gpio_1(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}

static int32_t stm32l5xx_gpio_b7_lock_pin_map_to_gpio_1(void)
{
    int32_t ret = HAL_SUCCESS;
    HAL_StatusTypeDef ret_x = \
        HAL_GPIO_LockPin(GPIOB, GPIO_PIN_7);
    if (ret_x != HAL_OK) {
        ret = HAL_ERR_HW_CONTROL;
    }

    return ret;
}

static int32_t stm32l5xx_gpio_c7_init_map_to_gpio_2(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = 0;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    return HAL_SUCCESS;
}

static void stm32l5xx_gpio_c7_deinit_map_to_gpio_2(void)
{
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);
}

static int32_t stm32l5xx_gpio_c7_read_pin_map_to_gpio_2(HAL_GPIO_PIN_STATE *res)
{
    int32_t ret = HAL_SUCCESS;
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);

    if (pin_state > GPIO_PIN_SET) {
        ret = HAL_ERR_UNSUPPORTED;
        goto finish;
    }

    *res = pin_state == GPIO_PIN_SET ? \
           HAL_GPIO_PIN_SET: \
           HAL_GPIO_PIN_RESET;
finish:
    return ret;
}

static void stm32l5xx_gpio_c7_write_pin_map_to_gpio_2(GPIO_PinState value)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, value);
}

static void stm32l5xx_gpio_c7_toggle_pin_map_to_gpio_2(void)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
}

static int32_t stm32l5xx_gpio_c7_lock_pin_map_to_gpio_2(void)
{
    int32_t ret = HAL_SUCCESS;
    HAL_StatusTypeDef ret_x = \
        HAL_GPIO_LockPin(GPIOC, GPIO_PIN_7);
    if (ret_x != HAL_OK) {
        ret = HAL_ERR_HW_CONTROL;
    }

    return ret;
}

// Initialize the specified GPIO pin according to the hal_gpio_init structure
int32_t hal_gpio_init(HAL_GPIO_CTX *gpio)
{
    int32_t ret = HAL_SUCCESS;

    if (NULL == gpio) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            ret = stm32l5xx_gpio_a9_init_map_to_gpio_0();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_1:
            ret = stm32l5xx_gpio_b7_init_map_to_gpio_1();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_2:
            ret = stm32l5xx_gpio_c7_init_map_to_gpio_2();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            goto finish;
    }

finish:
    return ret;
}

// Deinitialize the specified GPIO pin
void hal_gpio_deinit(HAL_GPIO_CTX *gpio)
{
    if (NULL == gpio) {
        return;
    }
    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            stm32l5xx_gpio_a9_deinit_map_to_gpio_0();
            return;
        case HAL_GPIO_1:
            stm32l5xx_gpio_b7_deinit_map_to_gpio_1();
            return;
        case HAL_GPIO_2:
            stm32l5xx_gpio_c7_deinit_map_to_gpio_2();
            return;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            return;
    }
}

// Read the input pin state
int32_t hal_gpio_read_pin(HAL_GPIO_CTX *gpio, HAL_GPIO_PIN_STATE *res)
{
    int32_t ret = HAL_SUCCESS;

    if ((NULL == gpio) ||
        (NULL == res)) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            ret = stm32l5xx_gpio_a9_read_pin_map_to_gpio_0(res);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_1:
            ret = stm32l5xx_gpio_b7_read_pin_map_to_gpio_1(res);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_2:
            ret = stm32l5xx_gpio_c7_read_pin_map_to_gpio_2(res);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            goto finish;
    }

finish:
    return ret;
}

// Write to the specified output pin
int32_t hal_gpio_write_pin(HAL_GPIO_CTX *gpio, HAL_GPIO_PIN_STATE state)
{
    int32_t ret = HAL_SUCCESS;

    // Null pointer check
    if (NULL == gpio) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }
    // Check if the state is valid
    if ((state != HAL_GPIO_PIN_SET) && (state != HAL_GPIO_PIN_RESET)) {
        ret = HAL_ERR_UNSUPPORTED;
        goto finish;
    }
    GPIO_PinState s = (state == HAL_GPIO_PIN_SET) ? \
                       GPIO_PIN_SET: \
                       GPIO_PIN_RESET;

    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            stm32l5xx_gpio_a9_write_pin_map_to_gpio_0(s);
            break;
        case HAL_GPIO_1:
            stm32l5xx_gpio_b7_write_pin_map_to_gpio_1(s);
            break;
        case HAL_GPIO_2:
            stm32l5xx_gpio_c7_write_pin_map_to_gpio_2(s);
            break;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            goto finish;
    }

finish:
    return ret;
}

// Toggle the specified GPIO pin
int32_t hal_gpio_toggle_pin(HAL_GPIO_CTX *gpio)
{
    int32_t ret = HAL_SUCCESS;

    // Null pointer check
    if (NULL == gpio) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            stm32l5xx_gpio_a9_toggle_pin_map_to_gpio_0();
            break;
        case HAL_GPIO_1:
            stm32l5xx_gpio_b7_toggle_pin_map_to_gpio_1();
            break;
        case HAL_GPIO_2:
            stm32l5xx_gpio_c7_toggle_pin_map_to_gpio_2();
            break;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            goto finish;
    }

finish:
    return ret;
}

// Lock the specified GPIO pin configuration
int32_t hal_gpio_lock_pin(HAL_GPIO_CTX *gpio)
{
    int32_t ret = HAL_SUCCESS;

    // Null pointer check
    if (NULL == gpio) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    switch(gpio->hal_num) {
        case HAL_GPIO_0:
            ret = stm32l5xx_gpio_a9_lock_pin_map_to_gpio_0();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_1:
            ret = stm32l5xx_gpio_b7_lock_pin_map_to_gpio_1();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_GPIO_2:
            ret = stm32l5xx_gpio_c7_lock_pin_map_to_gpio_2();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_GPIO_3:
        case HAL_GPIO_4:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            goto finish;
    }

finish:
    return ret;
}

#if HAL_UNIT_TEST
// Mock test function for hal_gpio
void hal_gpio_unit_test(void)
{
    size_t i = 1;

    HAL_GPIO_CTX blue_led_test_gpio = {
        .hal_num = HAL_GPIO_0,
    };

    HAL_GPIO_CTX red_led_test_gpio = {
        .hal_num = HAL_GPIO_1,
    };

    HAL_GPIO_CTX green_led_test_gpio = {
        .hal_num = HAL_GPIO_2,
    };

    // Test GPIO initialization
    if (hal_gpio_init(&blue_led_test_gpio) == HAL_SUCCESS) {
        printf("GPIO initialized successfully.\n");
    } else {
        printf("GPIO initialization failed.\n");
    }
    // Test GPIO initialization
    if (hal_gpio_init(&red_led_test_gpio) == HAL_SUCCESS) {
        printf("GPIO initialized successfully.\n");
    } else {
        printf("GPIO initialization failed.\n");
    }
    // Test GPIO initialization
    if (hal_gpio_init(&green_led_test_gpio) == HAL_SUCCESS) {
        printf("GPIO initialized successfully.\n");
    } else {
        printf("GPIO initialization failed.\n");
    }

    while (i) {
        // Test writing to GPIO pin
        if (hal_gpio_write_pin(&blue_led_test_gpio, HAL_GPIO_PIN_SET) == HAL_SUCCESS) {
            printf("GPIO write success (SET).\n");
        } else {
            printf("GPIO write failed.\n");
        }
        hal_delay_ms(200);
        // Test writing to GPIO pin
        if (hal_gpio_write_pin(&red_led_test_gpio, HAL_GPIO_PIN_SET) == HAL_SUCCESS) {
            printf("GPIO write success (SET).\n");
        } else {
            printf("GPIO write failed.\n");
        }
        hal_delay_ms(200);
        // Test writing to GPIO pin
        if (hal_gpio_write_pin(&green_led_test_gpio, HAL_GPIO_PIN_SET) == HAL_SUCCESS) {
            printf("GPIO write success (SET).\n");
        } else {
            printf("GPIO write failed.\n");
        }
        hal_delay_ms(200);
        hal_delay_ms(1000);
        // Test reading from GPIO pin
        enum hal_gpio_pin_state state;
        if (hal_gpio_read_pin(&green_led_test_gpio, &state) == HAL_SUCCESS) {
            printf("GPIO read success: %s\n", state == HAL_GPIO_PIN_SET ? "SET" : "RESET");
        } else {
            printf("GPIO read failed.\n");
        }
        hal_delay_ms(1000);
        // Test toggling the GPIO pin
        if (hal_gpio_toggle_pin(&blue_led_test_gpio) == HAL_SUCCESS) {
            printf("GPIO toggled successfully.\n");
        } else {
            printf("GPIO toggle failed.\n");
        }
        hal_delay_ms(200);
        if (hal_gpio_toggle_pin(&green_led_test_gpio) == HAL_SUCCESS) {
            printf("GPIO toggled successfully.\n");
        } else {
            printf("GPIO toggle failed.\n");
        }
        hal_delay_ms(200);
        if (hal_gpio_toggle_pin(&red_led_test_gpio) == HAL_SUCCESS) {
            printf("GPIO toggled successfully.\n");
        } else {
            printf("GPIO toggle failed.\n");
        }
        hal_delay_ms(1000);
        // Test locking the GPIO pin
        if (hal_gpio_lock_pin(&red_led_test_gpio) == HAL_SUCCESS) {
            printf("GPIO locked successfully.\n");
        } else {
            printf("GPIO lock failed.\n");
        }
         hal_delay_ms(1000);
         i --;
    }
    // Deinitialize the GPIO pin
    hal_gpio_deinit(&blue_led_test_gpio);
    hal_gpio_deinit(&green_led_test_gpio);
    hal_gpio_deinit(&red_led_test_gpio);
}
#endif /* HAL_UNIT_TEST */