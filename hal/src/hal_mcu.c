/**
 * @file hal_mcu.c
 * @brief MCU reset via SCB->AIRCR SYSRESETREQ (Cortex-M33).
 *
 * Mirrors FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_mcu.c (which pokes
 * S32_SCB directly); here we use the CMSIS SCB definition.
 */
#include "hal_mcu.h"
#include "stm32l5xx.h"

void hal_mcu_system_reset(void)
{
    __DSB();
    NVIC_SystemReset();   /* VECTKEY | keep PRIGROUP | SYSRESETREQ; does not return */
    for (;;) {
        __NOP();
    }
}
