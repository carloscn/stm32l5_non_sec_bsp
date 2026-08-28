/**
 * @file hal_flash.h
 * @brief HAL for on-chip non-volatile storage.
 *
 * API shape kept aligned with
 * FreeRTOS_Toggle_Led_Example_S32K312/hal/include/hal_flash.h (an A/B NVM
 * window of two erasable units). The STM32L552 has no separate "data flash":
 * this window is carved from the top of main flash Bank 2 and MUST be excluded
 * from the application in the linker script.
 *
 * STM32L552xE: 512 KiB main flash, dual bank, 2 KiB pages.
 *   Bank1 0x08000000..0x0803FFFF   Bank2 0x08040000..0x0807FFFF
 */
#ifndef HAL_FLASH_H
#define HAL_FLASH_H

#include <stddef.h>
#include <stdint.h>
#include "hal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_FLASH_BASE                (0x08000000UL)
#define HAL_FLASH_PAGE_SIZE          (2048U)          /* DBANK=1 */
#define HAL_FLASH_WORD_SIZE           (8U)            /* min program granularity (doubleword) */

/* A/B NVM slots: the last two pages of Bank 2. Keep the linker script's
 * application region ending at or below HAL_FLASH_NVM_SLOT_A_ADDR. */
#define HAL_FLASH_NVM_SLOT_A_ADDR    (0x0807F000UL)
#define HAL_FLASH_NVM_SLOT_B_ADDR    (0x0807F800UL)
#define HAL_FLASH_NVM_SLOT_SIZE      (HAL_FLASH_PAGE_SIZE)
#define HAL_FLASH_NVM_SLOT_COUNT     (2U)

/**
 * @brief One-time init. Unlocks nothing permanently; idempotent.
 * @return HAL_ERR_SUCCESS or a HAL_ERR_FLASH_* code.
 */
hal_err_t hal_flash_init(void);

/**
 * @brief Read bytes from a flash address (memory-mapped). @p addr..@p addr+len
 *        must lie inside the A/B NVM window.
 */
hal_err_t hal_flash_dflash_read(uint32_t addr, void *buf, size_t len);

/**
 * @brief Erase one NVM page. @p addr must equal SLOT_A or SLOT_B.
 */
hal_err_t hal_flash_dflash_erase_sector(uint32_t addr);

/**
 * @brief Program @p len bytes at @p addr. @p addr and @p len must be multiples
 *        of HAL_FLASH_WORD_SIZE (8) and lie in an erased page of the window.
 *        Read-back verified.
 */
hal_err_t hal_flash_dflash_program(uint32_t addr, const void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASH_H */
