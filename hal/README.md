# hal/ + osal/ — portable HAL and RTOS abstraction

Kept **API-synchronised** with
`s32k312_all/FreeRTOS_Toggle_Led_Example_S32K312` so application code (UDS,
diagnostics, business logic) moves between the S32K312 and STM32L552 targets
unchanged.

> **Sync note.** `osal/` `.c` files, `osal_error.[ch]`, `osal_types.h`,
> `hal_error.[ch]`, `hal_uart.h`, and the `hal_*.h` API contracts are kept
> byte-for-byte in step with the S32K312 project. Re-copy when it changes.
> STM32L5-only deviations: `osal_utils.c` clock constant (110 MHz);
> `hal_error.h` omits the `#define HAL_OK` alias (collides with ST HAL's
> `HAL_StatusTypeDef` enumerator) — use `HAL_ERR_SUCCESS` / `HAL_IS_OK()`;
> `hal_mcu.h` adds `hal_mcu_init()`; `osal_sched.h` adds `osal_panic()` /
> `osal_sched_kernel_version()`; `osal_hooks.c` is STM32L5-side.

## osal/  — FreeRTOS wrapper (portable, no MCU code)

| header | what |
|---|---|
| `osal_types.h`  | `osal_err_t`, `OSAL_OK/ERR/…`, `OSAL_WAIT_FOREVER/NONE` |
| `osal_task.h`   | create / delete / delay_ms / yield |
| `osal_mutex.h`  | opaque mutex (no-op success before scheduler start) |
| `osal_sem.h`    | binary + counting, `*_from_isr` |
| `osal_queue.h`  | message queue, `*_from_isr` |
| `osal_sched.h`  | `is_running`, critical section, `yield_from_isr`, `start`, `time_get_ms` |
| `osal_heap.h`   | `osal_malloc/calloc/free` over FreeRTOS heap_4 |
| `osal_log.h`    | deferred logging (LogTask + queue); blocking `SyncSend` before scheduler |
| `osal_utils.h`  | hex string, calibrated `delay_us/ms`, LE u32 helpers |

These `.c` files are **byte-identical** to the S32K312 project except
`osal_utils.c` (`CORE_CLOCK_HZ` 110 MHz vs 120 MHz).

## hal/  — thin, vendor-type-free peripheral API

| header | STM32L552 backing |
|---|---|
| `hal_error.h` | shared numeric codes (`HAL_ERR_*`, base `0x7000`) |
| `hal_gpio.h`  | ST HAL GPIO — NUCLEO-L552ZE-Q LD1 PC7 / LD2 PB7 / LD3 PA9, B1 PC13 |
| `hal_uart.h`  | ST HAL LPUART1 (PG7/PG8 → ST-Link VCP), blocking + IT + line editor |
| `hal_irq.h`   | CMSIS `NVIC_*` + RAM vector table for `hal_irq_install_handler` |
| `hal_mcu.h`   | `NVIC_SystemReset` |
| `hal_flash.h` | ST HAL FLASH — A/B NVM window = last two 2 KiB pages of Bank 2 |
| `hal_cache.h` | **no-op / barriers only** (Cortex-M33 has no D-cache); API kept for source parity with the M7 project |

## FreeRTOS

`FreeRTOS/` submodule, kernel **V11.1.0**, port
`GCC/ARM_CM33_NTZ/non_secure` (no TrustZone — this image is standalone,
TZEN=0), heap_4. Config in `config/FreeRTOSConfig.h` (110 MHz, 1 kHz tick).

SysTick is owned by the FreeRTOS port; the ST HAL 1 ms timebase runs on
**TIM6** (`Core/Src/stm32l5xx_hal_timebase_tim.c`).
