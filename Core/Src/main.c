/**
 ******************************************************************************
 * @file    main.c
 * @brief   Platform-agnostic application entry: HAL + OSAL only.
 *
 * No ST-HAL / CMSIS / FreeRTOS types here. MCU bring-up is hal_mcu_init();
 * RTOS glue is behind osal_*; each feature is its own module in app/.
 * Structure mirrors FreeRTOS_Toggle_Led_Example_S32K312/src/main.c.
 ******************************************************************************
 */
#include "hal_mcu.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "osal_log.h"
#include "osal_sched.h"

#include "led.h"
#include "heartbeat.h"

static HAL_UART s_console = { .num = HAL_UART_0, .irq = 0U };

int main(void)
{
    if (hal_mcu_init() != HAL_ERR_SUCCESS) {
        osal_panic("hal_mcu_init");
    }

    (void)hal_gpio_init();
    (void)hal_uart_init(&s_console);
    osal_log_init(&s_console);

    osal_log_printf("stm32l5_non_sec_bsp: FreeRTOS %s up",
                    osal_sched_kernel_version());

    led_start();
    heartbeat_start();

    osal_sched_start();      /* does not return */

    for (;;) {
    }
    return 0;
}
