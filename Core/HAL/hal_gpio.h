#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal_config.h"
#include "hal_utils.h"

typedef struct GPIO_TypeDef HAL_GPIO_PORT;

// Define GPIO mode types
enum hal_gpio_mode {
    HAL_GPIO_MODE_INPUT = 0x0,       // Input mode
    HAL_GPIO_MODE_OUTPUT_PP = 0x1,   // Output mode, push-pull
    HAL_GPIO_MODE_OUTPUT_OD = 0x2,   // Output mode, open-drain
    HAL_GPIO_MODE_ALT_PP = 0x3,      // Alternate function mode, push-pull
    HAL_GPIO_MODE_ALT_OD = 0x4,      // Alternate function mode, open-drain
    HAL_GPIO_MODE_ANALOG = 0x5       // Analog mode
};

// Define GPIO pull-up/pull-down types
enum hal_gpio_pull {
    HAL_GPIO_NOPULL = 0x0,           // No pull-up or pull-down
    HAL_GPIO_PULLUP = 0x1,           // Pull-up resistor enabled
    HAL_GPIO_PULLDOWN = 0x2          // Pull-down resistor enabled
};

// Define GPIO speed types
enum hal_gpio_speed {
    HAL_GPIO_SPEED_LOW = 0x0,        // Low speed
    HAL_GPIO_SPEED_MEDIUM = 0x1,     // Medium speed
    HAL_GPIO_SPEED_HIGH = 0x2,       // High speed
    HAL_GPIO_SPEED_VERY_HIGH = 0x3   // Very high speed
};

/**
 * @brief Enum for GPIO pin state.
 *
 * This enum defines the possible states of a GPIO pin:
 * either set (high) or reset (low).
 */
enum hal_gpio_pin_state {
    HAL_GPIO_PIN_RESET = 0x0,        // Pin is low
    HAL_GPIO_PIN_SET = 0x1           // Pin is high
};

typedef enum hal_gpio_interrupt_event {
    RISING = 0x0,
    FALLING,
    RISING_FALLING
} HAL_GPIO_INTE_EVENT;


typedef struct hal_gpio_ctx_t {
    HAL_GPIO_PORT *group;            // GPIO group (e.g., GPIOC)
    uint32_t pin;                    // Pin number (e.g., GPIO_PIN_0)
    enum hal_gpio_mode mode;         // GPIO mode (input, output, alternate, analog)
    enum hal_gpio_pull pull;         // Pull-up or pull-down setting
    enum hal_gpio_speed speed;       // GPIO speed
    uint32_t alternate;              // Alternate function mode (if applicable)
    uint32_t irq_number;
    void *interrupt_handler;
} HAL_GPIO_CTX;


/**
 * @brief Initialize the specified GPIO pin according to the context provided.
 *
 * This function configures the GPIO pin based on the parameters in the provided
 * HAL_GPIO_CTX structure. It sets the pin mode, pull configuration, speed, and
 * alternate function (if applicable).
 *
 * @param gpio Pointer to a HAL_GPIO_CTX structure containing the configuration details.
 * @return int32_t HAL_SUCCESS on success, HAL_ERROR_NULL_POINTER if the gpio pointer is NULL.
 */
int32_t hal_gpio_init(HAL_GPIO_CTX *gpio);

/**
 * @brief Deinitialize the specified GPIO pin.
 *
 * This function resets the GPIO pin configuration to its default state.
 * It effectively "un-configures" the pin.
 *
 * @param gpio Pointer to the HAL_GPIO_CTX structure for the pin to be deinitialized.
 */
void hal_gpio_deinit(HAL_GPIO_CTX *gpio);

/**
 * @brief Read the state of the specified GPIO pin.
 *
 * This function reads the logical state (set or reset) of the specified GPIO pin
 * and stores the result in the provided res pointer.
 *
 * @param gpio Pointer to the HAL_GPIO_CTX structure for the pin to be read.
 * @param res Pointer to an enum hal_gpio_pin_state variable where the result will be stored.
 * @return int32_t HAL_SUCCESS on success, HAL_ERROR_NULL_POINTER if the gpio or res pointer is NULL.
 */
int32_t hal_gpio_read_pin(HAL_GPIO_CTX *gpio, enum hal_gpio_pin_state *res);

/**
 * @brief Write a state to the specified GPIO pin.
 *
 * This function sets the output state of the GPIO pin to either set (high) or
 * reset (low) based on the provided state.
 *
 * @param gpio Pointer to the HAL_GPIO_CTX structure for the pin to be written.
 * @param state The desired state for the pin (HAL_GPIO_PIN_SET or HAL_GPIO_PIN_RESET).
 * @return int32_t HAL_SUCCESS on success, HAL_ERROR_NULL_POINTER if the gpio pointer is NULL,
 *                 or HAL_ERROR_UNSUPPORTED if the state is invalid.
 */
int32_t hal_gpio_write_pin(HAL_GPIO_CTX *gpio, enum hal_gpio_pin_state state);

/**
 * @brief Toggle the specified GPIO pin's state.
 *
 * This function toggles the current state of the GPIO pin. If the pin is currently
 * in a set state (high), it will be reset (low), and vice versa.
 *
 * @param gpio Pointer to the HAL_GPIO_CTX structure for the pin to be toggled.
 * @return int32_t HAL_SUCCESS on success, HAL_ERROR_NULL_POINTER if the gpio pointer is NULL.
 */
int32_t hal_gpio_toggle_pin(HAL_GPIO_CTX *gpio);

/**
 * @brief Lock the configuration of the specified GPIO pin.
 *
 * This function locks the current configuration of the GPIO pin, preventing
 * any further changes to the pin's configuration until a system reset.
 *
 * @param gpio Pointer to the HAL_GPIO_CTX structure for the pin to be locked.
 * @return int32_t HAL_SUCCESS on success, HAL_ERROR_NULL_POINTER if the gpio pointer is NULL,
 *                 or HAL_ERROR_HW_CONTROL if the hardware locking fails.
 */
int32_t hal_gpio_lock_pin(HAL_GPIO_CTX *gpio);

int32_t hal_gpio_reg_interrupt(HAL_GPIO_CTX *gpio, HAL_GPIO_INTE_EVENT event, void *handler_entry);


#if HAL_UNIT_TEST
/**
 * @brief Unit test function for GPIO functionality.
 *
 * This function serves as a basic unit test for the various GPIO operations,
 * including initialization, reading, writing, toggling, and locking the GPIO pin.
 * It provides print statements indicating whether each operation was successful or not.
 */
void hal_gpio_unit_test(void);
#endif /* HAL_UNIT_TEST */

#ifdef __cplusplus
}
#endif

#endif // _HAL_GPIO_H_
