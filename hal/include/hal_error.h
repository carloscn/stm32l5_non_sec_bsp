/**
 * @file hal_error.h
 * @brief HAL error codes (coordinated with OSAL).
 *
 * Convention (same as OSAL):
 *  - Success is always 0.
 *  - Failures are positive, unique codes in the HAL range [0x7000, 0x7FFF].
 *  - OSAL uses [0x6000, 0x6FFF] — ranges never overlap.
 *  - Callers test with HAL_IS_OK() / HAL_IS_ERR().
 *
 * Semantic pairs shared with OSAL (different numeric values, same meaning):
 *  INVALID_PARAM, NOT_INITIALIZED, TIMEOUT, RESOURCE_BUSY, NOT_SUPPORTED,
 *  OUT_OF_MEMORY, PERMISSION_DENIED, INTERNAL.
 */
#ifndef HAL_ERROR_H
#define HAL_ERROR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t hal_err_t;

/* ------------------------------------------------------------------------- */
/* Range                                                                     */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_BASE                        (0x7000)

/* ------------------------------------------------------------------------- */
/* Success                                                                   */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_SUCCESS                     (0)
/* NOTE (STM32L5 port deviation): the reference project also defines
 *   #define HAL_OK  HAL_ERR_SUCCESS
 * but on STM32 `HAL_OK` is an enumerator of ST HAL's HAL_StatusTypeDef, so
 * defining it as a macro breaks stm32l5xx_hal.h. Use HAL_ERR_SUCCESS /
 * HAL_IS_OK() here; ST HAL's own HAL_OK (also 0) is left alone. */

/* ------------------------------------------------------------------------- */
/* General                                                                   */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_INVALID_PARAM               (HAL_ERR_BASE + 1)
#define HAL_ERR_NOT_INITIALIZED             (HAL_ERR_BASE + 2)
#define HAL_ERR_TIMEOUT                     (HAL_ERR_BASE + 3)
#define HAL_ERR_RESOURCE_BUSY               (HAL_ERR_BASE + 4)
#define HAL_ERR_NOT_SUPPORTED               (HAL_ERR_BASE + 5)
#define HAL_ERR_OUT_OF_MEMORY               (HAL_ERR_BASE + 6)
#define HAL_ERR_PERMISSION_DENIED           (HAL_ERR_BASE + 7)
#define HAL_ERR_INTERNAL                    (HAL_ERR_BASE + 8)
#define HAL_ERR_FULL                        (HAL_ERR_BASE + 9)
#define HAL_ERR_EMPTY                       (HAL_ERR_BASE + 10)
#define HAL_ERR_ALREADY_INITIALIZED         (HAL_ERR_BASE + 11)
#define HAL_ERR_NOT_FOUND                   (HAL_ERR_BASE + 12)

/* ------------------------------------------------------------------------- */
/* SPI                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_SPI_INIT_FAILED             (HAL_ERR_BASE + 20)
#define HAL_ERR_SPI_XFER_FAILED             (HAL_ERR_BASE + 21)
#define HAL_ERR_SPI_INVALID_CONFIG          (HAL_ERR_BASE + 22)
#define HAL_ERR_SPI_INVALID_CHANNEL         (HAL_ERR_BASE + 23)

/* ------------------------------------------------------------------------- */
/* CAN                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_CAN_INIT_FAILED             (HAL_ERR_BASE + 30)
#define HAL_ERR_CAN_BUS_OFF                 (HAL_ERR_BASE + 31)
#define HAL_ERR_CAN_XFER_FAILED             (HAL_ERR_BASE + 32)
#define HAL_ERR_CAN_INVALID_ID              (HAL_ERR_BASE + 33)
#define HAL_ERR_CAN_BUFFER_FULL             (HAL_ERR_BASE + 34)
#define HAL_ERR_CAN_NOT_READY               (HAL_ERR_BASE + 35)

/* ------------------------------------------------------------------------- */
/* ADC                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_ADC_INIT_FAILED             (HAL_ERR_BASE + 40)
#define HAL_ERR_ADC_CONVERSION_FAILED       (HAL_ERR_BASE + 41)
#define HAL_ERR_ADC_INVALID_CHANNEL         (HAL_ERR_BASE + 42)
#define HAL_ERR_ADC_CALIBRATION_FAILED      (HAL_ERR_BASE + 43)

/* ------------------------------------------------------------------------- */
/* UART                                                                      */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_UART_INIT_FAILED            (HAL_ERR_BASE + 50)
#define HAL_ERR_UART_XFER_FAILED            (HAL_ERR_BASE + 51)
#define HAL_ERR_UART_PARITY_ERROR           (HAL_ERR_BASE + 52)
#define HAL_ERR_UART_FRAMING_ERROR          (HAL_ERR_BASE + 53)
#define HAL_ERR_UART_BUFFER_OVERFLOW        (HAL_ERR_BASE + 54)
#define HAL_ERR_UART_NOT_READY              (HAL_ERR_BASE + 55)

/* ------------------------------------------------------------------------- */
/* GPIO                                                                      */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_GPIO_INIT_FAILED            (HAL_ERR_BASE + 60)
#define HAL_ERR_GPIO_INVALID_PIN            (HAL_ERR_BASE + 61)
#define HAL_ERR_GPIO_CONFIG_FAILED          (HAL_ERR_BASE + 62)

/* ------------------------------------------------------------------------- */
/* HSE                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_HSE_INIT_FAILED             (HAL_ERR_BASE + 70)
#define HAL_ERR_HSE_KEY_INVALID             (HAL_ERR_BASE + 71)
#define HAL_ERR_HSE_CRYPTO_FAILED           (HAL_ERR_BASE + 72)
#define HAL_ERR_HSE_AUTH_FAILED             (HAL_ERR_BASE + 73)
#define HAL_ERR_HSE_NOT_READY               (HAL_ERR_BASE + 74)

/* ------------------------------------------------------------------------- */
/* PWM                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_PWM_INIT_FAILED             (HAL_ERR_BASE + 80)
#define HAL_ERR_PWM_INVALID_CHANNEL         (HAL_ERR_BASE + 81)
#define HAL_ERR_PWM_CONFIG_FAILED           (HAL_ERR_BASE + 82)

/* ------------------------------------------------------------------------- */
/* Timer                                                                     */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_TIMER_INIT_FAILED           (HAL_ERR_BASE + 90)
#define HAL_ERR_TIMER_INVALID_ID            (HAL_ERR_BASE + 91)
#define HAL_ERR_TIMER_CONFIG_FAILED         (HAL_ERR_BASE + 92)

/* ------------------------------------------------------------------------- */
/* Flash                                                                     */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_FLASH_INIT_FAILED           (HAL_ERR_BASE + 100)
#define HAL_ERR_FLASH_WRITE_FAILED          (HAL_ERR_BASE + 101)
#define HAL_ERR_FLASH_READ_FAILED           (HAL_ERR_BASE + 102)
#define HAL_ERR_FLASH_ERASE_FAILED          (HAL_ERR_BASE + 103)
#define HAL_ERR_FLASH_INVALID_ADDR          (HAL_ERR_BASE + 104)
#define HAL_ERR_FLASH_SECTOR_PROTECTED      (HAL_ERR_BASE + 105)
#define HAL_ERR_FLASH_VERIFY_FAILED         (HAL_ERR_BASE + 106)
#define HAL_ERR_FLASH_BUSY                  (HAL_ERR_BASE + 107)

/* ------------------------------------------------------------------------- */
/* IRQ / NVIC                                                                */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_IRQ_INIT_FAILED             (HAL_ERR_BASE + 110)
#define HAL_ERR_IRQ_INVALID                 (HAL_ERR_BASE + 111)
#define HAL_ERR_IRQ_HANDLER_INVALID         (HAL_ERR_BASE + 112)

/* ------------------------------------------------------------------------- */
/* Cache                                                                     */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_CACHE_OP_FAILED             (HAL_ERR_BASE + 120)
#define HAL_ERR_CACHE_INVALID_ADDR          (HAL_ERR_BASE + 121)

/* ------------------------------------------------------------------------- */
/* MCU                                                                       */
/* ------------------------------------------------------------------------- */
#define HAL_ERR_MCU_RESET_FAILED            (HAL_ERR_BASE + 130)
#define HAL_ERR_MCU_CLOCK_FAILED            (HAL_ERR_BASE + 131)

/* ------------------------------------------------------------------------- */
/* Helpers                                                                   */
/* ------------------------------------------------------------------------- */
#define HAL_IS_OK(err)                      ((hal_err_t)(err) == HAL_OK)
#define HAL_IS_ERR(err)                     ((hal_err_t)(err) != HAL_OK)

/**
 * Human-readable name for logging / asserts.
 * Always returns a non-NULL static string.
 */
const char *hal_err_str(hal_err_t err);

#ifdef __cplusplus
}
#endif

#endif /* HAL_ERROR_H */
