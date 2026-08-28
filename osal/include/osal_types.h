/**
 * @file osal_types.h
 * @brief Shared OSAL types / wait constants (no FreeRTOS in public headers).
 *
 * Error codes live in osal_error.h (included here so existing
 * `#include "osal_types.h"` keeps working).
 */
#ifndef OSAL_TYPES_H
#define OSAL_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "osal_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Wait forever (ms). Maps to FreeRTOS portMAX_DELAY internally. */
#define OSAL_WAIT_FOREVER  (0xFFFFFFFFUL)

/** Do not wait. */
#define OSAL_WAIT_NONE     (0U)

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TYPES_H */
