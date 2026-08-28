/**
 * @file hal_cache.h
 * @brief Data-buffer memory-ordering helpers.
 *
 * API kept identical to FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_cache.h
 * so buffer-staging code is portable.
 *
 * NOTE: the STM32L552 core (Cortex-M33) has NO data cache. These functions
 * reduce to DSB/ISB memory barriers. They are still worth calling around
 * DMA / bus-master buffer handoffs for the compiler/CPU ordering guarantee,
 * and they keep source shared with the S32K312 (Cortex-M7) project buildable
 * unchanged. There is an instruction cache (ICACHE) on the L5 but it is
 * transparent for data and needs no maintenance here.
 */
#ifndef HAL_CACHE_H
#define HAL_CACHE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Kept for source parity with the M7 project; not meaningful on M33. */
#define HAL_CACHE_LINE_SIZE (32U)

/** Barrier before another bus master reads host memory. (no-op cache-wise) */
void hal_cache_clean(const void *addr, size_t len);

/** Barrier after another bus master wrote host memory. (no-op cache-wise) */
void hal_cache_invalidate(const void *addr, size_t len);

/** clean + invalidate. (no-op cache-wise on M33) */
void hal_cache_flush(const void *addr, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* HAL_CACHE_H */
