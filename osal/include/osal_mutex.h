/**
 * @file osal_mutex.h
 * @brief Opaque mutex (FreeRTOS mutex under the hood).
 */
#ifndef OSAL_MUTEX_H
#define OSAL_MUTEX_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct osal_mutex *osal_mutex_t;

osal_mutex_t osal_mutex_create(void);
void osal_mutex_destroy(osal_mutex_t m);

/**
 * Take mutex. Before scheduler start: no-op success.
 * @param timeout_ms OSAL_WAIT_FOREVER / OSAL_WAIT_NONE / ms
 */
osal_err_t osal_mutex_take(osal_mutex_t m, uint32_t timeout_ms);
osal_err_t osal_mutex_give(osal_mutex_t m);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_MUTEX_H */
