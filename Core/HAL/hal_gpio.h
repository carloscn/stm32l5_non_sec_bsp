#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal_config.h"
#include "hal_utils.h"

/**
 * @brief Enum for GPIO pin state.
 *
 * This enum defines the possible states of a GPIO pin:
 * either set (high) or reset (low).
 */
typedef enum hal_gpio_pin_state {
    HAL_GPIO_PIN_RESET = 0x0,        // Pin is low
    HAL_GPIO_PIN_SET = 0x1           // Pin is high
} HAL_GPIO_PIN_STATE;

typedef enum hal_gpio_num {
    HAL_GPIO_0 = 0x0,
    HAL_GPIO_1,
    HAL_GPIO_2,
    HAL_GPIO_3,
    HAL_GPIO_4,
    HAL_GPIO_MAX
} HAL_GPIO_NUM;

typedef struct hal_gpio_ctx_t {
    HAL_GPIO_NUM hal_num;                    // Pin number (e.g., GPIO_PIN_0)
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
int32_t hal_gpio_read_pin(HAL_GPIO_CTX *gpio, HAL_GPIO_PIN_STATE *res);

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
int32_t hal_gpio_write_pin(HAL_GPIO_CTX *gpio, HAL_GPIO_PIN_STATE state);

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
