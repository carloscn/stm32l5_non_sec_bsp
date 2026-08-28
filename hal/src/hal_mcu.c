/**
 * @file hal_mcu.c
 * @brief MCU bring-up + reset + panic glue for STM32L552 (NUCLEO-L552ZE-Q).
 *
 * All the ST-HAL / clock-tree / cache code that used to sit in main.c lives
 * here so application code only ever touches hal_* / osal_*.
 */
#include "hal_mcu.h"
#include "osal_sched.h"          /* osal_panic */
#include "stm32l5xx_hal.h"

#if !defined(TFM_NS)
/* ------------------------------------------------------------- clock tree
 * MSI (RCC_MSIRANGE_6 = 4 MHz) -> PLL (M=1, N=55, R=2) => SYSCLK = 110 MHz.
 * Keep configCPU_CLOCK_HZ / osal_utils.c CORE_CLOCK_HZ in sync (110 MHz).
 * Not built for the NSPE (the SPE owns the clock tree).
 */
static hal_err_t clock_config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK) {
        return HAL_ERR_INTERNAL;
    }

    osc.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
    osc.MSIState            = RCC_MSI_ON;
    osc.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    osc.MSIClockRange       = RCC_MSIRANGE_6;
    osc.PLL.PLLState        = RCC_PLL_ON;
    osc.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
    osc.PLL.PLLM            = 1;
    osc.PLL.PLLN            = 55;
    osc.PLL.PLLP            = RCC_PLLP_DIV7;
    osc.PLL.PLLQ            = RCC_PLLQ_DIV2;
    osc.PLL.PLLR            = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
        return HAL_ERR_INTERNAL;
    }

    clk.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                         RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_5) != HAL_OK) {
        return HAL_ERR_INTERNAL;
    }
    return HAL_ERR_SUCCESS;
}
static hal_err_t icache_enable(void)
{
    if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK) {
        return HAL_ERR_INTERNAL;
    }
    if (HAL_ICACHE_Enable() != HAL_OK) {
        return HAL_ERR_INTERNAL;
    }
    return HAL_ERR_SUCCESS;
}
#endif /* !TFM_NS */

hal_err_t hal_mcu_init(void)
{
#if defined(TFM_NS)
    /* The SPE launches NS without programming the non-secure SCB->VTOR, so it
     * reads 0. The FreeRTOS CM33 port derives the initial MSP from *VTOR when
     * starting the first task -> reads addr 0 -> SecureFault -> reset loop.
     * Point it at our vector table (start of the NS image). */
    extern const uint32_t __VECTOR_TABLE[];
    SCB->VTOR = (uint32_t)__VECTOR_TABLE;
    __DSB();
    __ISB();

    /* Enable the NS configurable-fault handlers so a MemManage / BusFault /
     * UsageFault in NS thread mode vectors to our handlers (fault_ns.c) with a
     * CFSR dump, instead of escalating straight to the Secure HardFault (which
     * TF-M turns into a silent reboot). Bring-up diagnostics only. */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk |
                  SCB_SHCSR_BUSFAULTENA_Msk |
                  SCB_SHCSR_MEMFAULTENA_Msk;
    __DSB();
    __ISB();
#endif

    if (HAL_Init() != HAL_OK) {          /* NVIC group, HAL tick (TIM6 - see timebase file) */
        return HAL_ERR_INTERNAL;
    }

#if !defined(TFM_NS)
    /* As the TF-M NSPE the SPE already configured the clock tree / flash
     * latency / ICACHE; the NS world must not touch those (secure) registers. */
    hal_err_t rc = clock_config();
    if (rc != HAL_ERR_SUCCESS) {
        return rc;
    }
    return icache_enable();
#else
    return HAL_ERR_SUCCESS;
#endif
}

void hal_mcu_system_reset(void)
{
    __DSB();
    NVIC_SystemReset();                  /* does not return */
    for (;;) {
        __NOP();
    }
}

/* --------------------------------------------------------- ST HAL required glue
 * stm32l5xx_hal_msp.c calls Error_Handler(); route it (and assert) to osal_panic.
 */
void Error_Handler(void)
{
    osal_panic("ST HAL Error_Handler");
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
    osal_panic("assert_param failed");
}
#endif
