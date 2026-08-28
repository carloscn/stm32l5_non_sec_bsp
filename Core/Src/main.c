/**
 ******************************************************************************
 * @file    main.c
 * @brief   Standalone (no-TF-M) application entry: HAL + OSAL only.
 *
 * No ST-HAL / CMSIS / FreeRTOS types. MCU bring-up is hal_mcu_init(); RTOS
 * glue is behind osal_*. Every task is created HERE (one place to see what
 * runs); each feature module only provides its task function.
 * Structure mirrors FreeRTOS_Toggle_Led_Example_S32K312/src/main.c.
 ******************************************************************************
 */
#include "hal_mcu.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "osal_log.h"
#include "osal_sched.h"
#include "osal_task.h"

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

    /* --- all task creation lives here --- */
    if (osal_task_create(led_task, "led",
                         LED_TASK_STACK_WORDS, NULL, LED_TASK_PRIORITY, NULL) != OSAL_OK ||
        osal_task_create(heartbeat_task, "hb",
                         HEARTBEAT_TASK_STACK_WORDS, NULL, HEARTBEAT_TASK_PRIORITY, NULL) != OSAL_OK) {
        osal_panic("task create");
    }

    osal_sched_start();      /* does not return */

    for (;;) {
    }
    return 0;
}
