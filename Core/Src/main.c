/**
 ******************************************************************************
 * @file    main.c
 * @brief   Platform-agnostic application entry: HAL + OSAL only.
 *
 * No ST-HAL / CMSIS / FreeRTOS types here. All MCU bring-up is in
 * hal_mcu_init(); all RTOS glue is behind osal_*.
 * Structure mirrors FreeRTOS_Toggle_Led_Example_S32K312/src/main.c.
 ******************************************************************************
 */
#include "hal_mcu.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "osal_log.h"
#include "osal_sched.h"
#include "osal_task.h"
#include "osal_heap.h"

#define PRIO_LED         (2U)
#define PRIO_HEARTBEAT   (2U)
#define STACK_LED        (128U)
#define STACK_HEARTBEAT  (256U)
#define HEARTBEAT_MS     (1000U)

static HAL_UART s_console = { .num = HAL_UART_0, .irq = 0U };

/* ------------------------------------------------------------------ tasks */

static void led_task(void *arg)
{
    (void)arg;
    for (;;) {
        hal_gpio_toggle(HAL_GPIO_LED_GREEN);
        osal_task_delay_ms(250U);
    }
}

static void heartbeat_task(void *arg)
{
    uint32_t n = 0U;
    (void)arg;
    for (;;) {
        osal_log_printf("heartbeat %lu  (heap free %u)",
                        (unsigned long)n++, (unsigned)osal_heap_get_free());
        osal_task_delay_ms(HEARTBEAT_MS);
    }
}

/* ------------------------------------------------------------------ main */

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

    if (osal_task_create(led_task, "led", STACK_LED, NULL, PRIO_LED, NULL) != OSAL_OK ||
        osal_task_create(heartbeat_task, "hb", STACK_HEARTBEAT, NULL,
                         PRIO_HEARTBEAT, NULL) != OSAL_OK) {
        osal_panic("task create");
    }

    osal_sched_start();      /* does not return */

    for (;;) {
    }
    return 0;
}
