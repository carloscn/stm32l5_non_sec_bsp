/**
 * @file hal_irq.c
 * @brief NVIC wrapper (CMSIS) + RAM vector table for runtime handler install.
 *
 * Same API as FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_irq.c
 * (that one wraps NXP IntCtrl_Ip; here plain CMSIS NVIC_*).
 */
#include "hal_irq.h"
#include "stm32l5xx.h"

/* STM32L552: 16 system exceptions + up to ~110 IRQs. Round up. */
#define HAL_IRQ_VECTOR_COUNT   (16U + 110U)

/* SCB->VTOR requires the table aligned to a power of two >= table size. */
static void (*s_ram_vectors[HAL_IRQ_VECTOR_COUNT])(void)
    __attribute__((aligned(512)));

static uint8_t s_inited;

hal_err_t hal_irq_init(void)
{
    if (0U != s_inited) {
        return HAL_ERR_SUCCESS;
    }

    const uint32_t *src = (const uint32_t *)SCB->VTOR;
    for (uint32_t i = 0U; i < HAL_IRQ_VECTOR_COUNT; i++) {
        s_ram_vectors[i] = (void (*)(void))src[i];
    }

    __DMB();
    SCB->VTOR = (uint32_t)s_ram_vectors;
    __DSB();
    __ISB();

    s_inited = 1U;
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_irq_enable(uint8_t irq)
{
    NVIC_EnableIRQ((IRQn_Type)irq);
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_irq_disable(uint8_t irq)
{
    NVIC_DisableIRQ((IRQn_Type)irq);
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_irq_set_priority(uint8_t irq, uint8_t priority)
{
    NVIC_SetPriority((IRQn_Type)irq, priority);
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_irq_install_handler(uint8_t irq, hal_irq_handler_t handler)
{
    if (NULL == handler) {
        return HAL_ERR_INVALID_PARAM;
    }
    if (0U == s_inited) {
        (void)hal_irq_init();
    }
    s_ram_vectors[16U + irq] = handler;
    __DSB();
    return HAL_ERR_SUCCESS;
}

void hal_irq_global_enable(void)
{
    __enable_irq();
}

void hal_irq_global_disable(void)
{
    __disable_irq();
}
