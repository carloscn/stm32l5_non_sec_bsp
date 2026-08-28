/**
 * @file hal_error.h
 * @brief HAL Error Codes (shared with FreeRTOS_Toggle_Led_Example_S32K312)
 * @details Error codes for the Hardware Abstraction Layer (HAL). Kept
 *          numerically identical to the S32K312 project so HAL-consuming code
 *          is portable across both. STM32L552 implementation sits on ST LL/HAL.
 */

#ifndef HAL_ERROR_H
#define HAL_ERROR_H

/* HAL error base to avoid conflicts with OSAL or system errors */
#define HAL_ERR_BASE                    (0x7000)

/* HAL General Errors */
#define HAL_ERR_SUCCESS                 (0)                             /* Operation successful */
#define HAL_ERR_INVALID_PARAM           (HAL_ERR_BASE + 1)             /* Invalid parameter passed to function */
#define HAL_ERR_NOT_INITIALIZED         (HAL_ERR_BASE + 2)             /* Module not initialized */
#define HAL_ERR_TIMEOUT                 (HAL_ERR_BASE + 3)             /* Hardware operation timed out */
#define HAL_ERR_RESOURCE_BUSY           (HAL_ERR_BASE + 4)             /* Hardware resource is busy */
#define HAL_ERR_NOT_SUPPORTED           (HAL_ERR_BASE + 5)             /* Operation not supported */
#define HAL_ERR_OUT_OF_MEMORY           (HAL_ERR_BASE + 6)             /* Memory allocation failed */
#define HAL_ERR_PERMISSION_DENIED       (HAL_ERR_BASE + 7)             /* Access to hardware denied */
#define HAL_ERR_INTERNAL                (HAL_ERR_BASE + 8)             /* Internal HAL error */

/* SPI Errors */
#define HAL_ERR_SPI_INIT_FAILED         (HAL_ERR_BASE + 20)            /* SPI initialization failed */
#define HAL_ERR_SPI_XFER_FAILED         (HAL_ERR_BASE + 21)            /* SPI transfer failed */
#define HAL_ERR_SPI_INVALID_CONFIG      (HAL_ERR_BASE + 22)            /* Invalid SPI configuration */
#define HAL_ERR_SPI_INVALID_CHANNEL     (HAL_ERR_BASE + 23)            /* Invalid SPI channel */

/* CAN Errors */
#define HAL_ERR_CAN_INIT_FAILED         (HAL_ERR_BASE + 30)            /* CAN initialization failed */
#define HAL_ERR_CAN_BUS_OFF             (HAL_ERR_BASE + 31)            /* CAN bus off error */
#define HAL_ERR_CAN_XFER_FAILED         (HAL_ERR_BASE + 32)            /* CAN transfer failed */
#define HAL_ERR_CAN_INVALID_ID          (HAL_ERR_BASE + 33)            /* Invalid CAN message ID */
#define HAL_ERR_CAN_BUFFER_FULL         (HAL_ERR_BASE + 34)            /* CAN buffer full */

/* ADC Errors */
#define HAL_ERR_ADC_INIT_FAILED         (HAL_ERR_BASE + 40)            /* ADC initialization failed */
#define HAL_ERR_ADC_CONVERSION_FAILED   (HAL_ERR_BASE + 41)            /* ADC conversion failed */
#define HAL_ERR_ADC_INVALID_CHANNEL     (HAL_ERR_BASE + 42)            /* Invalid ADC channel */
#define HAL_ERR_ADC_CALIBRATION_FAILED  (HAL_ERR_BASE + 43)            /* ADC calibration failed */

/* UART Errors */
#define HAL_ERR_UART_INIT_FAILED        (HAL_ERR_BASE + 50)            /* UART initialization failed */
#define HAL_ERR_UART_XFER_FAILED        (HAL_ERR_BASE + 51)            /* UART transfer failed */
#define HAL_ERR_UART_PARITY_ERROR       (HAL_ERR_BASE + 52)            /* UART parity error */
#define HAL_ERR_UART_FRAMING_ERROR      (HAL_ERR_BASE + 53)            /* UART framing error */
#define HAL_ERR_UART_BUFFER_OVERFLOW    (HAL_ERR_BASE + 54)            /* UART buffer overflow */

/* GPIO Errors */
#define HAL_ERR_GPIO_INIT_FAILED        (HAL_ERR_BASE + 60)            /* GPIO initialization failed */
#define HAL_ERR_GPIO_INVALID_PIN        (HAL_ERR_BASE + 61)            /* Invalid GPIO pin */
#define HAL_ERR_GPIO_CONFIG_FAILED      (HAL_ERR_BASE + 62)            /* GPIO configuration failed */

/* HSE (Hardware Security Engine) Errors */
#define HAL_ERR_HSE_INIT_FAILED         (HAL_ERR_BASE + 70)            /* HSE initialization failed */
#define HAL_ERR_HSE_KEY_INVALID         (HAL_ERR_BASE + 71)            /* Invalid HSE key */
#define HAL_ERR_HSE_CRYPTO_FAILED       (HAL_ERR_BASE + 72)            /* HSE cryptographic operation failed */
#define HAL_ERR_HSE_AUTH_FAILED         (HAL_ERR_BASE + 73)            /* HSE authentication failed */

/* PWM Errors */
#define HAL_ERR_PWM_INIT_FAILED         (HAL_ERR_BASE + 80)            /* PWM initialization failed */
#define HAL_ERR_PWM_INVALID_CHANNEL     (HAL_ERR_BASE + 81)            /* Invalid PWM channel */
#define HAL_ERR_PWM_CONFIG_FAILED       (HAL_ERR_BASE + 82)            /* PWM configuration failed */

/* Timer Errors */
#define HAL_ERR_TIMER_INIT_FAILED       (HAL_ERR_BASE + 90)            /* Timer initialization failed */
#define HAL_ERR_TIMER_INVALID_ID        (HAL_ERR_BASE + 91)            /* Invalid timer ID */
#define HAL_ERR_TIMER_CONFIG_FAILED     (HAL_ERR_BASE + 92)            /* Timer configuration failed */

/* Flash Errors */
#define HAL_ERR_FLASH_INIT_FAILED       (HAL_ERR_BASE + 100)           /* Flash initialization failed */
#define HAL_ERR_FLASH_WRITE_FAILED      (HAL_ERR_BASE + 101)           /* Flash write operation failed */
#define HAL_ERR_FLASH_READ_FAILED       (HAL_ERR_BASE + 102)           /* Flash read operation failed */
#define HAL_ERR_FLASH_ERASE_FAILED      (HAL_ERR_BASE + 103)           /* Flash erase operation failed */
#define HAL_ERR_FLASH_INVALID_ADDR      (HAL_ERR_BASE + 104)           /* Invalid flash address */
#define HAL_ERR_FLASH_SECTOR_PROTECTED  (HAL_ERR_BASE + 105)           /* Flash sector is protected */
#define HAL_ERR_FLASH_VERIFY_FAILED     (HAL_ERR_BASE + 106)           /* Flash verify operation failed */

/* Error type */
typedef int32_t hal_err_t;

#endif /* HAL_ERROR_H */
