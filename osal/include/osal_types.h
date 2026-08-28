/**
 * @file osal_types.h
 * @brief Shared OSAL types / wait constants (no FreeRTOS in public headers).
 */
#ifndef OSAL_TYPES_H
#define OSAL_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Wait forever (ms). Maps to FreeRTOS portMAX_DELAY internally. */
#define OSAL_WAIT_FOREVER  (0xFFFFFFFFUL)

/** Do not wait. */
#define OSAL_WAIT_NONE     (0U)

/** Generic OSAL result. 0 = OK. */
typedef int32_t osal_err_t;

#define OSAL_OK            (0)
#define OSAL_ERR           (-1)
#define OSAL_ERR_TIMEOUT   (-2)
#define OSAL_ERR_NOMEM     (-3)
#define OSAL_ERR_PARAM     (-4)

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TYPES_H */
