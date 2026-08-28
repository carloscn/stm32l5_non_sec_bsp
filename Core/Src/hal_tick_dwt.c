/**
 * @file hal_tick_dwt.c
 * @brief ST HAL time base on the DWT cycle counter (NS build only).
 *
 * The FreeRTOS ARM_CM33_NTZ port owns SysTick, and the NS world must not touch
 * the (secure) TIM clocks. DWT->CYCCNT is free-running, needs no interrupt and
 * works before the scheduler starts, which is all HAL_GetTick() needs here.
 */
#if defined(TFM_NS)

#include "stm32l5xx_hal.h"

/* ARMv8-M CMSIS renamed CoreDebug -> DCB. */
#ifndef DCB
#define DCB CoreDebug
#endif
#ifndef DCB_DEMCR_TRCENA_Msk
#define DCB_DEMCR_TRCENA_Msk CoreDebug_DEMCR_TRCENA_Msk
#endif

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    (void)TickPriority;

    DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;    /* enable trace/DWT */
    DWT->CYCCNT = 0U;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;  /* start the cycle counter */

    uwTickPrio = TickPriority;
    return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
    uint32_t cyc_per_ms = SystemCoreClock / 1000U;
    if (cyc_per_ms == 0U) {
        cyc_per_ms = 110000U;         /* 110 MHz fallback */
    }
    return DWT->CYCCNT / cyc_per_ms;
}

void HAL_Delay(uint32_t Delay)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < Delay) {
        __NOP();
    }
}

void HAL_SuspendTick(void)  { }
void HAL_ResumeTick(void)   { }

#endif /* TFM_NS */
