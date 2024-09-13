#include "hal_err.h"
#include "hal_gpio.h"
#include "stm32l5xx.h"
#include "stm32l5xx_hal_gpio.h"
#include <stdio.h>

// Initialize the specified GPIO pin according to the hal_gpio_init structure
int32_t hal_gpio_init(HAL_GPIO_CTX *gpio)
{
    int32_t ret = HAL_SUCCESS;

    if (NULL == gpio) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = gpio->pin;
    gpio_init.Mode = gpio->mode;
    gpio_init.Pull = gpio->pull;
    gpio_init.Speed = gpio->speed;
    gpio_init.Alternate = gpio->alternate;

    HAL_GPIO_Init((GPIO_TypeDef *)gpio->group, &gpio_init);

    gpio->interrupt_handler = NULL;
finish:
    return ret;
}

// Deinitialize the specified GPIO pin
void hal_gpio_deinit(HAL_GPIO_CTX *gpio)
{
    if (NULL == gpio) {
        return;
    }
    HAL_GPIO_DeInit((GPIO_TypeDef *)gpio->group, gpio->pin);
}

// Read the input pin state
int32_t hal_gpio_read_pin(HAL_GPIO_CTX *gpio, enum hal_gpio_pin_state *res)
{
    int32_t ret = HAL_SUCCESS;

    if ((NULL == gpio) ||
        (NULL == res)) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }
    GPIO_PinState pin_state = HAL_GPIO_ReadPin((GPIO_TypeDef *)gpio->group,
                                               (uint16_t)gpio->pin);
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

// Write to the specified output pin
int32_t hal_gpio_write_pin(HAL_GPIO_CTX *gpio, enum hal_gpio_pin_state state)
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

    // Write the pin state
    HAL_GPIO_WritePin((GPIO_TypeDef *)gpio->group, (uint16_t)gpio->pin, (GPIO_PinState)state);

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

    // Toggle the GPIO pin
    HAL_GPIO_TogglePin((GPIO_TypeDef *)gpio->group, (uint16_t)gpio->pin);

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

    // Lock the GPIO pin configuration
    HAL_StatusTypeDef ret_x = \
        HAL_GPIO_LockPin((GPIO_TypeDef *)gpio->group, (uint16_t)gpio->pin);
    if (ret_x != HAL_OK) {
        ret = HAL_ERR_HW_CONTROL;
    }

finish:
    return ret;
}

// Function to register an interrupt for a GPIO pin
int32_t hal_gpio_reg_interrupt(HAL_GPIO_CTX *gpio, HAL_GPIO_INTE_EVENT event, void *handler_entry)
{
    int32_t ret = HAL_SUCCESS;

    if (gpio == NULL || handler_entry == NULL) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    uint32_t mode = 0;
    if (event == FALLING) {
        mode = GPIO_MODE_IT_FALLING;
    } else if (event == RISING) {
        mode = GPIO_MODE_IT_RISING;
    } else if (event == RISING_FALLING) {
        mode = GPIO_MODE_IT_RISING_FALLING;
    }else {
        ret = HAL_ERR_UNSUPPORTED;
        goto finish;
    }
    // Save the interrupt handler in the context
    gpio->interrupt_handler = handler_entry;

    // Configure the EXTI line for the GPIO pin
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = gpio->pin;
    gpio_init.Mode = gpio->mode;
    gpio_init.Pull = gpio->pull;
    gpio_init.Speed = gpio->speed;
    gpio_init.Alternate = gpio->alternate;
    HAL_GPIO_Init(gpio->group, &gpio_init);


finish:
    return ret;
}

// Function to unregister the interrupt for a GPIO pin
int32_t hal_gpio_unreg_interrupt(HAL_GPIO_CTX *gpio)
{
    if (gpio == NULL) {
        return HAL_ERR_NULL_POINTER;
    }

    // Disable the EXTI line associated with the GPIO pin
    uint32_t irq_number = 0;
    GPIO_InitTypeDef   GPIO_InitStructure;


    return HAL_SUCCESS;
}

#if HAL_UNIT_TEST
// Mock test function for hal_gpio
void hal_gpio_unit_test(void)
{
    size_t i = 1000;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_CTX blue_led_test_gpio = {
        .group = (HAL_GPIO_PORT *)GPIOB,
        .pin = GPIO_PIN_7,
        .mode = GPIO_MODE_OUTPUT_PP,
        .pull = GPIO_PULLUP,
        .speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = 0
    };

    HAL_GPIO_CTX red_led_test_gpio = {
        .group = (HAL_GPIO_PORT *)GPIOA,
        .pin = GPIO_PIN_9,
        .mode = GPIO_MODE_OUTPUT_PP,
        .pull = GPIO_PULLUP,
        .speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = 0
    };

    HAL_GPIO_CTX green_led_test_gpio = {
        .group = (HAL_GPIO_PORT *)GPIOC,
        .pin = GPIO_PIN_7,
        .mode = GPIO_MODE_OUTPUT_PP,
        .pull = GPIO_PULLUP,
        .speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = 0
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