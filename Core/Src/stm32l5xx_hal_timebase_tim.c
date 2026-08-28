/**
 ******************************************************************************
 * @file    stm32l5xx_hal_timebase_tim.c
 * @brief   ST HAL time base on TIM6 (1 kHz), so SysTick is free for the
 *          FreeRTOS ARM_CM33_NTZ port. Based on the STM32Cube template.
 ******************************************************************************
 */
#include "stm32l5xx_hal.h"

#if !defined(TFM_NS)   /* NS build uses the DWT timebase (hal_tick_dwt.c) */

static TIM_HandleTypeDef s_htim6;

/* Called by HAL_Init() (and after each clock change via HAL_ResumeTick). */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock;
    uint32_t uwAPB1Prescaler;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;
    HAL_StatusTypeDef status;

    /* TIM6 IRQ priority: strictly below the FreeRTOS max-syscall threshold is
     * not required (it makes no FreeRTOS calls), but keep it low-urgency. */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
        HAL_NVIC_SetPriority(TIM6_IRQn, TickPriority, 0U);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
        uwTickPrio = TickPriority;
    } else {
        return HAL_ERROR;
    }

    __HAL_RCC_TIM6_CLK_ENABLE();
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1) {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* 1 MHz TIM6 counter, 1 kHz update. */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    s_htim6.Instance = TIM6;
    s_htim6.Init.Period            = (1000000U / 1000U) - 1U;
    s_htim6.Init.Prescaler         = uwPrescalerValue;
    s_htim6.Init.ClockDivision     = 0U;
    s_htim6.Init.CounterMode       = TIM_COUNTERMODE_UP;
    s_htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&s_htim6);
    if (status == HAL_OK) {
        status = HAL_TIM_Base_Start_IT(&s_htim6);
    }
    return status;
}

void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&s_htim6, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&s_htim6, TIM_IT_UPDATE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
}

void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&s_htim6);
}

#endif /* !TFM_NS */
