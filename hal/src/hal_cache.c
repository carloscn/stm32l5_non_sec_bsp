/**
 * @file hal_cache.c
 * @brief Cortex-M33 has no data cache -> these are memory barriers only.
 *
 * Same API as FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_cache.c so
 * buffer-staging code compiles unchanged on both. See hal_cache.h.
 */
#include "hal_cache.h"
#include "stm32l5xx.h"

void hal_cache_clean(const void *addr, size_t len)
{
    (void)addr;
    (void)len;
    __DSB();
}

void hal_cache_invalidate(const void *addr, size_t len)
{
    (void)addr;
    (void)len;
    __DSB();
    __ISB();
}

void hal_cache_flush(const void *addr, size_t len)
{
    (void)addr;
    (void)len;
    __DSB();
    __ISB();
}
