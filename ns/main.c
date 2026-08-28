/**
 ******************************************************************************
 * @file    ns/main.c
 * @brief   TF-M Non-Secure Processing Environment entry.
 *
 * Runs under the SPE built by ../stm32l5_secure_bsp. Differences from the
 * standalone Core/Src/main.c:
 *   - NO clock config here (the SPE set the clock tree; NS inherits it).
 *   - tfm_ns_interface_init() + psa_crypto_init() before any psa_* call.
 *   - crypto_smoketest_run() runs the PSA KATs before the scheduler starts.
 *
 * All task creation is in this file (single place), same rule as the
 * standalone main.
 ******************************************************************************
 */
#include "hal_gpio.h"
#include "hal_uart.h"
#include "osal_log.h"
#include "osal_sched.h"
#include "osal_task.h"

#include "led.h"
#include "heartbeat.h"
#include "crypto_smoketest.h"

#include "tfm_ns_interface.h"

static HAL_UART s_console = { .num = HAL_UART_0, .irq = 0U };

int main(void)
{
    /* SPE already configured clocks/flash/caches. NS just needs its peripherals. */
    (void)hal_gpio_init();
    (void)hal_uart_init(&s_console);
    osal_log_init(&s_console);

    osal_log_printf("stm32l5_non_sec_bsp (NSPE): FreeRTOS %s up",
                    osal_sched_kernel_version());

    if (tfm_ns_interface_init() != 0) {
        osal_panic("tfm_ns_interface_init");
    }

    /* PSA crypto smoke test (CMAC + ECDSA P-256) via the TF-M Crypto partition. */
    (void)crypto_smoketest_run();

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
