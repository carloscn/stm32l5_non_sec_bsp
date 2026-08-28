/*******************************************************************************
 * @file hal_uart.c
 * @brief UART HAL for STM32L552 (LPUART1 -> ST-Link VCP, PG7/PG8).
 *
 * Behavioural twin of FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_uart.c:
 *   - hal_uart_transmit       : blocking, real-ms timeout
 *   - hal_uart_transmit_it    : async start + yield-poll to completion (RTOS safe)
 *   - hal_uart_receive        : async start + 1ms-quantum poll, real-ms timeout
 *   - hal_uart_receive_it     : async start, non-blocking; poll via _get_receive_status
 *   - hal_uart_read_line      : byte-wise line editor with echo
 *   - weak tx/rx complete callbacks, driven from LPUART1_IRQHandler
 *
 * The single console instance is LPUART1 regardless of huart->num.
 ******************************************************************************/
#include "hal_uart.h"
#include "hal_error.h"
#include "osal_sched.h"
#include "osal_task.h"
#include "osal_utils.h"
#include "stm32l5xx_hal.h"
#include <string.h>

static UART_HandleTypeDef s_lpuart1;
static HAL_UART          *s_huart;

static volatile bool s_tx_busy;
static volatile bool s_rx_busy;
static size_t        s_rx_size;

/* ------------------------------------------------------------------ init */

static int32_t lpuart1_msp_init(void)
{
    GPIO_InitTypeDef gi = {0};
    RCC_PeriphCLKInitTypeDef pclk = {0};

    pclk.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    pclk.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&pclk) != HAL_OK) {
        return HAL_ERR_UART_INIT_FAILED;
    }

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();               /* PG[15:2] rail */
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_LPUART1_CLK_ENABLE();

    gi.Pin       = GPIO_PIN_7 | GPIO_PIN_8; /* PG7 TX, PG8 RX */
    gi.Mode      = GPIO_MODE_AF_PP;
    gi.Pull      = GPIO_NOPULL;
    gi.Speed     = GPIO_SPEED_FREQ_LOW;
    gi.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOG, &gi);

    return HAL_ERR_SUCCESS;
}

hal_err_t hal_uart_init(HAL_UART *huart)
{
    if (huart == NULL || huart->num >= HAL_UART_NUM_MAX) {
        return HAL_ERR_INVALID_PARAM;
    }

    int32_t rc = lpuart1_msp_init();
    if (rc != HAL_ERR_SUCCESS) {
        return rc;
    }

    s_lpuart1.Instance                    = LPUART1;
    s_lpuart1.Init.BaudRate               = 115200U;
    s_lpuart1.Init.WordLength             = UART_WORDLENGTH_8B;
    s_lpuart1.Init.StopBits               = UART_STOPBITS_1;
    s_lpuart1.Init.Parity                 = UART_PARITY_NONE;
    s_lpuart1.Init.Mode                   = UART_MODE_TX_RX;
    s_lpuart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    s_lpuart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    s_lpuart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    s_lpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    s_lpuart1.FifoMode                    = UART_FIFOMODE_DISABLE;

    if (HAL_UART_Init(&s_lpuart1) != HAL_OK) {
        return HAL_ERR_UART_INIT_FAILED;
    }

    HAL_NVIC_SetPriority(LPUART1_IRQn, 6, 0);   /* >= configMAX_SYSCALL prio */
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);

    s_huart   = huart;
    huart->irq = (uint8_t)LPUART1_IRQn;
    s_tx_busy = false;
    s_rx_busy = false;
    return HAL_ERR_SUCCESS;
}

void hal_uart_deinit(HAL_UART *huart)
{
    (void)huart;
    HAL_NVIC_DisableIRQ(LPUART1_IRQn);
    (void)HAL_UART_DeInit(&s_lpuart1);
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7 | GPIO_PIN_8);
    s_huart = NULL;
    s_tx_busy = false;
    s_rx_busy = false;
}

/* ------------------------------------------------------------- blocking TX/RX */

hal_err_t hal_uart_transmit(HAL_UART *huart, uint8_t *data, size_t size, size_t timeout_ms)
{
    if (huart == NULL || data == NULL || size == 0U) {
        return HAL_ERR_INVALID_PARAM;
    }
    if (s_tx_busy) {
        return HAL_ERR_RESOURCE_BUSY;
    }
    HAL_StatusTypeDef st = HAL_UART_Transmit(&s_lpuart1, data, (uint16_t)size,
                                             (uint32_t)timeout_ms);
    if (st == HAL_TIMEOUT) {
        return HAL_ERR_TIMEOUT;
    }
    return (st == HAL_OK) ? HAL_ERR_SUCCESS : HAL_ERR_UART_XFER_FAILED;
}

hal_err_t hal_uart_receive(HAL_UART *huart, uint8_t *data, size_t size, size_t timeout_ms)
{
    if (huart == NULL || data == NULL || size == 0U) {
        return HAL_ERR_INVALID_PARAM;
    }
    if (s_rx_busy) {
        return HAL_ERR_RESOURCE_BUSY;
    }

    s_rx_busy = true;
    s_rx_size = size;
    if (HAL_UART_Receive_IT(&s_lpuart1, data, (uint16_t)size) != HAL_OK) {
        s_rx_busy = false;
        return HAL_ERR_UART_XFER_FAILED;
    }

    size_t elapsed = 0U;
    while (s_rx_busy) {
        if (elapsed >= timeout_ms) {
            hal_uart_abort_receive(huart);
            return HAL_ERR_TIMEOUT;
        }
        if (osal_sched_is_running()) {
            osal_task_delay_ms(1);
        } else {
            osal_utils_delay_ms(1);
        }
        elapsed++;
    }
    return HAL_ERR_SUCCESS;
}

/* ------------------------------------------------------------- interrupt TX/RX */

hal_err_t hal_uart_transmit_it(HAL_UART *huart, uint8_t *data, size_t size)
{
    if (huart == NULL || data == NULL || size == 0U) {
        return HAL_ERR_INVALID_PARAM;
    }
    if (s_tx_busy) {
        return HAL_ERR_RESOURCE_BUSY;
    }

    s_tx_busy = true;
    if (HAL_UART_Transmit_IT(&s_lpuart1, data, (uint16_t)size) != HAL_OK) {
        s_tx_busy = false;
        return HAL_ERR_UART_XFER_FAILED;
    }

    /* Yield-poll to completion so callers can treat TX as synchronous while
     * the scheduler keeps running (same contract as the S32K version). */
    while (s_tx_busy) {
        if (osal_sched_is_running()) {
            osal_task_yield();
        }
    }
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_uart_receive_it(HAL_UART *huart, uint8_t *data, size_t size)
{
    if (huart == NULL || data == NULL || size == 0U) {
        return HAL_ERR_INVALID_PARAM;
    }
    if (s_rx_busy) {
        return HAL_ERR_RESOURCE_BUSY;
    }
    s_rx_busy = true;
    s_rx_size = size;
    if (HAL_UART_Receive_IT(&s_lpuart1, data, (uint16_t)size) != HAL_OK) {
        s_rx_busy = false;
        return HAL_ERR_UART_XFER_FAILED;
    }
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_uart_get_receive_status(HAL_UART *huart, size_t *bytes_remaining)
{
    if (huart == NULL || bytes_remaining == NULL) {
        return HAL_ERR_INVALID_PARAM;
    }
    *bytes_remaining = (size_t)s_lpuart1.RxXferCount;
    return s_rx_busy ? HAL_ERR_RESOURCE_BUSY : HAL_ERR_SUCCESS;
}

void hal_uart_abort_receive(HAL_UART *huart)
{
    (void)huart;
    (void)HAL_UART_AbortReceive(&s_lpuart1);
    s_rx_busy = false;
}

/* --------------------------------------------------------------- line editor */

hal_err_t hal_uart_read_line(HAL_UART *huart, char *buf, size_t buf_size,
                           size_t idle_timeout_ms, size_t *out_len)
{
    if (huart == NULL || buf == NULL || buf_size < 2U || out_len == NULL) {
        return HAL_ERR_INVALID_PARAM;
    }

    size_t len = 0U;
    for (;;) {
        uint8_t ch = 0U;
        int32_t st = hal_uart_receive(huart, &ch, 1U, idle_timeout_ms);
        if (st != HAL_ERR_SUCCESS) {
            return st;
        }
        if (ch == '\r' || ch == '\n') {
            (void)hal_uart_transmit_it(huart, (uint8_t *)"\r\n", 2U);
            break;
        } else if (ch == 0x08U || ch == 0x7FU) {
            if (len > 0U) {
                len--;
                (void)hal_uart_transmit_it(huart, (uint8_t *)"\b \b", 3U);
            }
        } else if (ch >= 0x20U && ch < 0x7FU) {
            if (len < (buf_size - 1U)) {
                buf[len++] = (char)ch;
                (void)hal_uart_transmit_it(huart, &ch, 1U);
            }
        }
    }
    buf[len] = '\0';
    *out_len = len;
    return HAL_ERR_SUCCESS;
}

/* --------------------------------------------------------------- ISR glue */

void LPUART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&s_lpuart1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *h)
{
    if (h->Instance == LPUART1) {
        s_tx_busy = false;
        hal_uart_tx_cplt_callback(s_huart);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *h)
{
    if (h->Instance == LPUART1) {
        s_rx_busy = false;
        hal_uart_rx_cplt_callback(s_huart);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *h)
{
    if (h->Instance == LPUART1) {
        s_tx_busy = false;
        s_rx_busy = false;
    }
}

__attribute__((weak)) void hal_uart_tx_cplt_callback(HAL_UART *huart) { (void)huart; }
__attribute__((weak)) void hal_uart_rx_cplt_callback(HAL_UART *huart) { (void)huart; }

#if defined(HAL_UNIT_TEST) && HAL_UNIT_TEST
void hal_uart_unit_test(void)
{
    HAL_UART u = { .num = HAL_UART_0, .irq = 0U };
    if (hal_uart_init(&u) == HAL_ERR_SUCCESS) {
        (void)hal_uart_transmit(&u, (uint8_t *)"hal_uart ok\r\n", 13U, 100U);
    }
}
#endif
