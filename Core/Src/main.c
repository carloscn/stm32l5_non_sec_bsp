/**
 ******************************************************************************
 * @file    main.c
 * @brief   Application entry -- HAL + OSAL only. One file for both builds:
 *            - standalone (no TF-M): make
 *            - TF-M NSPE (TFM_NS defined): CMakeLists.txt (see ns/README.md)
 *
 * No ST-HAL / CMSIS / FreeRTOS types here. MCU bring-up is hal_mcu_init()
 * (which itself skips the clock tree when running as the NSPE -- the SPE
 * already set it). Every task is created HERE, one place.
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

#if defined(TFM_NS)
#include "tfm_ns_interface.h"
#include "test_psa_cmac.h"
#include "test_psa_ecdsa_p256.h"
#endif

static HAL_UART s_console = { .num = HAL_UART_0, .irq = 0U };

int main(void)
{
    if (hal_mcu_init() != HAL_ERR_SUCCESS) {
        osal_panic("hal_mcu_init");
    }

    (void)hal_gpio_init();
    (void)hal_uart_init(&s_console);
    osal_log_init(&s_console);

    osal_log_printf("stm32l5_non_sec_bsp%s: FreeRTOS %s up",
#if defined(TFM_NS)
                    " (NSPE)",
#else
                    "",
#endif
                    osal_sched_kernel_version());

#if defined(TFM_NS)
    if (tfm_ns_interface_init() != 0) {
        osal_panic("tfm_ns_interface_init");
    }
    /* PSA crypto smoke test via the TF-M Crypto partition. */
    (void)test_psa_cmac();
    (void)test_psa_ecdsa_p256();
#endif

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
