/**
 * @file hal_flash.c
 * @brief STM32L552 main-flash NVM window (A/B slots) via ST HAL FLASH.
 *
 * API shape mirrors FreeRTOS_Toggle_Led_Example_S32K312/hal/src/hal_flash.c.
 * The window is the last two 2 KiB pages of Bank 2 (see hal_flash.h) and MUST
 * be kept out of the application's linker region.
 */
#include "hal_flash.h"
#include <string.h>
#include "stm32l5xx_hal.h"

#define NVM_WIN_START   HAL_FLASH_NVM_SLOT_A_ADDR
#define NVM_WIN_END     (HAL_FLASH_NVM_SLOT_B_ADDR + HAL_FLASH_NVM_SLOT_SIZE)

static uint8_t s_inited;

static int in_window(uint32_t addr, size_t len)
{
    return (addr >= NVM_WIN_START) && (len <= (size_t)(NVM_WIN_END - NVM_WIN_START)) &&
           ((addr + len) <= NVM_WIN_END);
}

/* Bank2 page index for a window address (DBANK=1: 2 KiB pages, 128 pages/bank). */
static uint32_t bank2_page(uint32_t addr)
{
    return (addr - 0x08040000UL) / HAL_FLASH_PAGE_SIZE;
}

hal_err_t hal_flash_init(void)
{
    s_inited = 1U;
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_flash_dflash_read(uint32_t addr, void *buf, size_t len)
{
    if (0U == s_inited || NULL == buf) {
        return HAL_ERR_NOT_INITIALIZED;
    }
    if (!in_window(addr, len)) {
        return HAL_ERR_FLASH_INVALID_ADDR;
    }
    (void)memcpy(buf, (const void *)(uintptr_t)addr, len);
    return HAL_ERR_SUCCESS;
}

hal_err_t hal_flash_dflash_erase_sector(uint32_t addr)
{
    FLASH_EraseInitTypeDef e = {0};
    uint32_t page_err = 0U;
    hal_err_t rc = HAL_ERR_SUCCESS;

    if (0U == s_inited) {
        return HAL_ERR_NOT_INITIALIZED;
    }
    if (addr != HAL_FLASH_NVM_SLOT_A_ADDR && addr != HAL_FLASH_NVM_SLOT_B_ADDR) {
        return HAL_ERR_FLASH_INVALID_ADDR;
    }

    e.TypeErase = FLASH_TYPEERASE_PAGES;
    e.Banks     = FLASH_BANK_2;
    e.Page      = bank2_page(addr);
    e.NbPages   = 1U;

    if (HAL_OK != HAL_FLASH_Unlock()) {
        return HAL_ERR_FLASH_ERASE_FAILED;
    }
    if (HAL_OK != HAL_FLASHEx_Erase(&e, &page_err)) {
        rc = HAL_ERR_FLASH_ERASE_FAILED;
    }
    (void)HAL_FLASH_Lock();
    return rc;
}

hal_err_t hal_flash_dflash_program(uint32_t addr, const void *buf, size_t len)
{
    const uint8_t *p = (const uint8_t *)buf;
    hal_err_t rc = HAL_ERR_SUCCESS;

    if (0U == s_inited || NULL == buf) {
        return HAL_ERR_NOT_INITIALIZED;
    }
    if (!in_window(addr, len) ||
        (addr % HAL_FLASH_WORD_SIZE) != 0U || (len % HAL_FLASH_WORD_SIZE) != 0U) {
        return HAL_ERR_FLASH_INVALID_ADDR;
    }

    if (HAL_OK != HAL_FLASH_Unlock()) {
        return HAL_ERR_FLASH_WRITE_FAILED;
    }
    for (size_t off = 0U; off < len; off += HAL_FLASH_WORD_SIZE) {
        uint64_t dw;
        (void)memcpy(&dw, &p[off], HAL_FLASH_WORD_SIZE);
        if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + off, dw)) {
            rc = HAL_ERR_FLASH_WRITE_FAILED;
            break;
        }
    }
    (void)HAL_FLASH_Lock();

    if (HAL_ERR_SUCCESS == rc &&
        0 != memcmp((const void *)(uintptr_t)addr, buf, len)) {
        rc = HAL_ERR_FLASH_VERIFY_FAILED;
    }
    return rc;
}
