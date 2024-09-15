#include <string.h>
#include "hal_uart.h"
#include "hal_err.h"
#include "stm32l552xx.h"
#include "stm32l5xx_hal.h"
#include "stm32l5xx_hal_uart.h"
#include "stm32l5xx_hal_uart_ex.h"
#include "stm32l5xx_hal_gpio_ex.h"

static UART_HandleTypeDef lpuart1; // map to HAL_UART_1
static UART_HandleTypeDef huart2;  // map to HAL_UART_2

static int32_t stm32l5xx_lpuart1_init_map_to_hal_uart_1(void)
{
    int32_t ret = HAL_SUCCESS;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }

    __HAL_RCC_LPUART1_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();
    /**LPUART1 GPIO Configuration
    PG7     ------> LPUART1_TX
    PG8     ------> LPUART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    lpuart1.Instance = LPUART1;
    lpuart1.Init.BaudRate = 115200;
    lpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    lpuart1.Init.StopBits = UART_STOPBITS_1;
    lpuart1.Init.Parity = UART_PARITY_NONE;
    lpuart1.Init.Mode = UART_MODE_TX_RX;
    lpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    lpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    lpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    lpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    lpuart1.FifoMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&lpuart1) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&lpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&lpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_DisableFifoMode(&lpuart1) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }

    return ret;
}

static void stm32l5xx_lpuart1_deinit_map_to_hal_uart_1(void)
{
    __HAL_RCC_LPUART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7 | GPIO_PIN_8);
    HAL_UART_DeInit(&lpuart1);
}

static int32_t stm32l5xx_lpuart1_send_data_map_to_hal_uart_1(uint8_t *buffer, size_t len, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    HAL_StatusTypeDef status = HAL_UART_Transmit(&lpuart1, buffer, len, timeout_ms);
    if (HAL_OK == status) {
        ret = HAL_SUCCESS;
    } else if (HAL_BUSY == status) {
        ret = HAL_ERR_BUSY;
    } else if (HAL_TIMEOUT == status) {
        ret = HAL_ERR_TIMEOUT;
    } else {
        ret = HAL_ERR_HW_SEND;
    }

    return ret;
}

static int32_t stm32l5xx_lpuart1_recv_data_map_to_hal_uart_1(uint8_t *buffer, size_t len, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    HAL_StatusTypeDef status = HAL_UART_Receive(&lpuart1, buffer, len, timeout_ms);
    if (HAL_OK == status) {
        ret = HAL_SUCCESS;
    } else if (HAL_BUSY == status) {
        ret = HAL_ERR_BUSY;
    } else if (HAL_TIMEOUT == status) {
        ret = HAL_ERR_TIMEOUT;
    } else {
        ret = HAL_ERR_HW_RECV;
    }

    return ret;
}

static int32_t stm32l5xx_uart2_init_map_to_hal_uart_2(void)
{
    int32_t ret = HAL_SUCCESS;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**USART2 GPIO Configuration

    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
        ret = HAL_ERR_INIT;
        return ret;
    }

    return ret;
}

static void stm32l5xx_uart2_deinit_map_to_hal_uart_2(void)
{
    __HAL_RCC_USART2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
    HAL_UART_DeInit(&huart2);
}

static int32_t stm32l5xx_uart2_send_data_map_to_hal_uart_2(uint8_t *buffer, size_t len, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, buffer, len, timeout_ms);
    if (HAL_OK == status) {
        ret = HAL_SUCCESS;
    } else if (HAL_BUSY == status) {
        ret = HAL_ERR_BUSY;
    } else if (HAL_TIMEOUT == status) {
        ret = HAL_ERR_TIMEOUT;
    } else {
        ret = HAL_ERR_HW_SEND;
    }

    return ret;
}

static int32_t stm32l5xx_uart2_recv_data_map_to_hal_uart_2(uint8_t *buffer, size_t len, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, buffer, len, timeout_ms);
    if (HAL_OK == status) {
        ret = HAL_SUCCESS;
    } else if (HAL_BUSY == status) {
        ret = HAL_ERR_BUSY;
    } else if (HAL_TIMEOUT == status) {
        ret = HAL_ERR_TIMEOUT;
    } else {
        ret = HAL_ERR_HW_RECV;
    }

    return ret;
}

int32_t hal_uart_init(HAL_UART *huart)
{
    int32_t ret = 0;

    if (huart == NULL) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    // Set the UART instance based on the UART number
    switch (huart->num) {
        case HAL_UART_1:
            ret = stm32l5xx_lpuart1_init_map_to_hal_uart_1();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_UART_2:
            ret = stm32l5xx_uart2_init_map_to_hal_uart_2();
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_UART_3:
        case HAL_UART_4:
        case HAL_UART_5:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            break;
    }

finish:

    return HAL_OK;
}

void hal_uart_deinit(HAL_UART *huart)
{
    if (huart == NULL) {
        return;
    }
    // Set the UART instance based on the UART number
    switch (huart->num) {
        case HAL_UART_1:
            stm32l5xx_lpuart1_deinit_map_to_hal_uart_1();
            break;
        case HAL_UART_2:
            stm32l5xx_uart2_deinit_map_to_hal_uart_2();
            break;
#if LOCK_CODE
        case HAL_UART_3:
        case HAL_UART_4:
        case HAL_UART_5:
#endif /* LOCK_CODE */
        default:
            break;
    }
}

int32_t hal_uart_transmit(HAL_UART *huart, uint8_t *data, uint16_t size, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    if (NULL == huart || NULL == data) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    if (0 == size) {
        ret = HAL_SUCCESS;
        goto finish;
    }

    // Set the UART instance based on the UART number
    switch (huart->num) {
        case HAL_UART_1:
            ret = stm32l5xx_lpuart1_send_data_map_to_hal_uart_1(data, size, timeout_ms);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_UART_2:
            ret = stm32l5xx_uart2_send_data_map_to_hal_uart_2(data, size, timeout_ms);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_UART_3:
        case HAL_UART_4:
        case HAL_UART_5:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            break;
    }

finish:
    return ret;
}

int32_t hal_uart_receive(HAL_UART *huart, uint8_t *data, uint16_t size, size_t timeout_ms)
{
    int32_t ret = HAL_SUCCESS;

    if (NULL == huart || NULL == data) {
        ret = HAL_ERR_NULL_POINTER;
        goto finish;
    }

    if (0 == size) {
        ret = HAL_SUCCESS;
        goto finish;
    }

    // Set the UART instance based on the UART number
    switch (huart->num) {
        case HAL_UART_1:
            ret = stm32l5xx_lpuart1_recv_data_map_to_hal_uart_1(data, size, timeout_ms);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
        case HAL_UART_2:
            ret = stm32l5xx_uart2_recv_data_map_to_hal_uart_2(data, size, timeout_ms);
            if (ret != HAL_SUCCESS) {
                goto finish;
            }
            break;
#if LOCK_CODE
        case HAL_UART_3:
        case HAL_UART_4:
        case HAL_UART_5:
#endif /* LOCK_CODE */
        default:
            ret = HAL_ERR_HW_LACK;
            break;
    }

finish:
    return ret;
}

int32_t hal_uart_transmit_it(HAL_UART *huart, uint8_t *data, size_t size)
{
    if (huart == NULL || data == NULL) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

int32_t hal_uart_receive_it(HAL_UART *huart, uint8_t *data, size_t size)
{
    if (huart == NULL || data == NULL) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

void hal_uart_tx_cplt_callback(HAL_UART *huart)
{
    if (huart->send_handler != NULL) {
        void (*handler)(void) = huart->send_handler;
      handler();
    }
}

void hal_uart_rx_cplt_callback(HAL_UART *huart)
{
    if (huart->recv_handler != NULL) {
        void (*handler)(void) = huart->recv_handler;
      handler();
    }
}

void hal_uart_irq_handler(HAL_UART *huart)
{

}

#if HAL_UNIT_TEST
/**
 * @brief Unit test for HAL UART.
 *
 * This function tests the UART initialization, deinitialization, transmission, and reception.
 */
void hal_uart_unit_test(void)
{
    int32_t ret = 0;
    HAL_UART u1_ctx = {
        .num = HAL_UART_1
    };
    HAL_UART u2_ctx = {
        .num = HAL_UART_2
    };

    char *hello_send = "hello!\n";
    char  hello_recv[8] = {0};
    size_t timeout_ms = 100;

    ret = hal_uart_init(&u1_ctx);
    if (ret != 0) {
        return;
    }

    ret = hal_uart_transmit(&u1_ctx, (uint8_t *)hello_send, strlen(hello_send), timeout_ms);
    if (ret != 0) {
        return;
    }

    ret = hal_uart_receive(&u1_ctx, (uint8_t *)hello_recv, strlen(hello_send), timeout_ms);
    if (ret != 0) {
        return;
    }

    hal_uart_deinit(&u1_ctx);

    ret = hal_uart_init(&u2_ctx);
    if (ret != 0) {
        return;
    }

    ret = hal_uart_transmit(&u2_ctx, (uint8_t *)hello_send, strlen(hello_send), timeout_ms);
    if (ret != 0) {
        return;
    }

    ret = hal_uart_receive(&u2_ctx, (uint8_t *)hello_recv, strlen(hello_send), timeout_ms);
    if (ret != 0) {
        return;
    }

    hal_uart_deinit(&u2_ctx);
}
#endif /* HAL_UNIT_TEST */
