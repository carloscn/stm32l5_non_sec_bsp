/**
 * @file hal_uart.h
 * @brief Hardware Abstraction Layer (HAL) for UART operations.
 *
 * Status-returning APIs use @ref hal_err_t (see hal_error.h).
 */
#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include <stdint.h>
#include <stddef.h>
#include "hal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum HAL_UART_NUM_T{
	HAL_UART_0 = 0,
    HAL_UART_1,
    HAL_UART_2,
    HAL_UART_3,
    HAL_UART_4,
    HAL_UART_5,
	HAL_UART_6,
    HAL_UART_NUM_MAX
} HAL_UART_NUM;

typedef struct hal_uart_t {
    HAL_UART_NUM num;
    uint8_t irq;
} HAL_UART;

/**
 * @brief Initialize the specified UART peripheral.
 * @return HAL_OK on success, otherwise a HAL_ERR_* code.
 */
hal_err_t hal_uart_init(HAL_UART *huart);

/**
 * @brief Deinitialize the specified UART peripheral.
 */
void hal_uart_deinit(HAL_UART *huart);

/**
 * @brief Blocking transmit.
 * @return HAL_OK on success, otherwise a HAL_ERR_* code.
 */
hal_err_t hal_uart_transmit(HAL_UART *huart, uint8_t *data, size_t size, size_t timeout_ms);

/**
 * @brief Blocking receive.
 * @return HAL_OK on success, otherwise a HAL_ERR_* code.
 */
hal_err_t hal_uart_receive(HAL_UART *huart, uint8_t *data, size_t size, size_t timeout_ms);

/**
 * @brief Read a line (local echo, CR/LF terminated).
 *
 * idle_timeout_ms is the max gap between consecutive bytes.
 * @return HAL_OK, HAL_ERR_TIMEOUT, or HAL_ERR_INVALID_PARAM.
 */
hal_err_t hal_uart_read_line(HAL_UART *huart, char *buf, size_t buf_size,
                             size_t idle_timeout_ms, size_t *out_len);

/**
 * @brief Interrupt-driven transmit.
 * @return HAL_OK on success, otherwise a HAL_ERR_* code.
 */
hal_err_t hal_uart_transmit_it(HAL_UART *huart, uint8_t *data, size_t size);

/**
 * @brief Interrupt-driven receive.
 * @return HAL_OK on success, otherwise a HAL_ERR_* code.
 */
hal_err_t hal_uart_receive_it(HAL_UART *huart, uint8_t *data, size_t size);

/**
 * @brief Poll an in-flight AsyncReceive from @ref hal_uart_receive_it.
 * @return HAL_OK when complete, HAL_ERR_RESOURCE_BUSY while in progress,
 *         or another HAL_ERR_* on failure.
 */
hal_err_t hal_uart_get_receive_status(HAL_UART *huart, size_t *bytes_remaining);

/**
 * @brief Abort an in-flight receive and clear HAL RX busy state.
 */
void hal_uart_abort_receive(HAL_UART *huart);

void hal_uart_tx_cplt_callback(HAL_UART *huart);
void hal_uart_rx_cplt_callback(HAL_UART *huart);

#if defined(HAL_UNIT_TEST) && HAL_UNIT_TEST
void hal_uart_unit_test(void);
#endif /* HAL_UNIT_TEST */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_UART_H_ */
