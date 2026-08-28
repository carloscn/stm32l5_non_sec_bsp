/**
 * @file hal_error.c
 * @brief hal_err_str() — keep names in sync with hal_error.h.
 */
#include "hal_error.h"

const char *hal_err_str(hal_err_t err)
{
    switch (err) {
    case HAL_ERR_SUCCESS:                 return "OK";
    case HAL_ERR_INVALID_PARAM:           return "INVALID_PARAM";
    case HAL_ERR_NOT_INITIALIZED:         return "NOT_INITIALIZED";
    case HAL_ERR_TIMEOUT:                 return "TIMEOUT";
    case HAL_ERR_RESOURCE_BUSY:           return "RESOURCE_BUSY";
    case HAL_ERR_NOT_SUPPORTED:           return "NOT_SUPPORTED";
    case HAL_ERR_OUT_OF_MEMORY:           return "OUT_OF_MEMORY";
    case HAL_ERR_PERMISSION_DENIED:       return "PERMISSION_DENIED";
    case HAL_ERR_INTERNAL:               return "INTERNAL";
    case HAL_ERR_FULL:                   return "FULL";
    case HAL_ERR_EMPTY:                  return "EMPTY";
    case HAL_ERR_ALREADY_INITIALIZED:    return "ALREADY_INITIALIZED";
    case HAL_ERR_NOT_FOUND:              return "NOT_FOUND";

    case HAL_ERR_SPI_INIT_FAILED:         return "SPI_INIT_FAILED";
    case HAL_ERR_SPI_XFER_FAILED:         return "SPI_XFER_FAILED";
    case HAL_ERR_SPI_INVALID_CONFIG:      return "SPI_INVALID_CONFIG";
    case HAL_ERR_SPI_INVALID_CHANNEL:     return "SPI_INVALID_CHANNEL";

    case HAL_ERR_CAN_INIT_FAILED:         return "CAN_INIT_FAILED";
    case HAL_ERR_CAN_BUS_OFF:             return "CAN_BUS_OFF";
    case HAL_ERR_CAN_XFER_FAILED:         return "CAN_XFER_FAILED";
    case HAL_ERR_CAN_INVALID_ID:          return "CAN_INVALID_ID";
    case HAL_ERR_CAN_BUFFER_FULL:         return "CAN_BUFFER_FULL";
    case HAL_ERR_CAN_NOT_READY:           return "CAN_NOT_READY";

    case HAL_ERR_ADC_INIT_FAILED:         return "ADC_INIT_FAILED";
    case HAL_ERR_ADC_CONVERSION_FAILED:   return "ADC_CONVERSION_FAILED";
    case HAL_ERR_ADC_INVALID_CHANNEL:     return "ADC_INVALID_CHANNEL";
    case HAL_ERR_ADC_CALIBRATION_FAILED:  return "ADC_CALIBRATION_FAILED";

    case HAL_ERR_UART_INIT_FAILED:        return "UART_INIT_FAILED";
    case HAL_ERR_UART_XFER_FAILED:        return "UART_XFER_FAILED";
    case HAL_ERR_UART_PARITY_ERROR:       return "UART_PARITY_ERROR";
    case HAL_ERR_UART_FRAMING_ERROR:      return "UART_FRAMING_ERROR";
    case HAL_ERR_UART_BUFFER_OVERFLOW:    return "UART_BUFFER_OVERFLOW";
    case HAL_ERR_UART_NOT_READY:          return "UART_NOT_READY";

    case HAL_ERR_GPIO_INIT_FAILED:        return "GPIO_INIT_FAILED";
    case HAL_ERR_GPIO_INVALID_PIN:        return "GPIO_INVALID_PIN";
    case HAL_ERR_GPIO_CONFIG_FAILED:      return "GPIO_CONFIG_FAILED";

    case HAL_ERR_HSE_INIT_FAILED:         return "HSE_INIT_FAILED";
    case HAL_ERR_HSE_KEY_INVALID:         return "HSE_KEY_INVALID";
    case HAL_ERR_HSE_CRYPTO_FAILED:       return "HSE_CRYPTO_FAILED";
    case HAL_ERR_HSE_AUTH_FAILED:         return "HSE_AUTH_FAILED";
    case HAL_ERR_HSE_NOT_READY:           return "HSE_NOT_READY";

    case HAL_ERR_PWM_INIT_FAILED:         return "PWM_INIT_FAILED";
    case HAL_ERR_PWM_INVALID_CHANNEL:     return "PWM_INVALID_CHANNEL";
    case HAL_ERR_PWM_CONFIG_FAILED:       return "PWM_CONFIG_FAILED";

    case HAL_ERR_TIMER_INIT_FAILED:       return "TIMER_INIT_FAILED";
    case HAL_ERR_TIMER_INVALID_ID:        return "TIMER_INVALID_ID";
    case HAL_ERR_TIMER_CONFIG_FAILED:     return "TIMER_CONFIG_FAILED";

    case HAL_ERR_FLASH_INIT_FAILED:       return "FLASH_INIT_FAILED";
    case HAL_ERR_FLASH_WRITE_FAILED:      return "FLASH_WRITE_FAILED";
    case HAL_ERR_FLASH_READ_FAILED:       return "FLASH_READ_FAILED";
    case HAL_ERR_FLASH_ERASE_FAILED:      return "FLASH_ERASE_FAILED";
    case HAL_ERR_FLASH_INVALID_ADDR:      return "FLASH_INVALID_ADDR";
    case HAL_ERR_FLASH_SECTOR_PROTECTED:  return "FLASH_SECTOR_PROTECTED";
    case HAL_ERR_FLASH_VERIFY_FAILED:     return "FLASH_VERIFY_FAILED";
    case HAL_ERR_FLASH_BUSY:              return "FLASH_BUSY";

    case HAL_ERR_IRQ_INIT_FAILED:         return "IRQ_INIT_FAILED";
    case HAL_ERR_IRQ_INVALID:             return "IRQ_INVALID";
    case HAL_ERR_IRQ_HANDLER_INVALID:     return "IRQ_HANDLER_INVALID";

    case HAL_ERR_CACHE_OP_FAILED:         return "CACHE_OP_FAILED";
    case HAL_ERR_CACHE_INVALID_ADDR:      return "CACHE_INVALID_ADDR";

    case HAL_ERR_MCU_RESET_FAILED:        return "MCU_RESET_FAILED";
    case HAL_ERR_MCU_CLOCK_FAILED:        return "MCU_CLOCK_FAILED";

    default:                              return "UNKNOWN";
    }
}
