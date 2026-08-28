/**
 * @file osal_task.h
 * @brief Task create / delay / delete.
 */
#ifndef OSAL_TASK_H
#define OSAL_TASK_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*osal_task_fn_t)(void *arg);
typedef struct osal_task *osal_task_t;

/**
 * Create a FreeRTOS task.
 * @param stack_words stack depth in words (same unit as xTaskCreate).
 * @param priority 0 = idle+0; typically use small offsets from idle.
 * @param out_handle optional; may be NULL.
 */
osal_err_t osal_task_create(osal_task_fn_t fn, const char *name,
                            uint32_t stack_words, void *arg,
                            uint32_t priority, osal_task_t *out_handle);

void osal_task_delete(osal_task_t t);
void osal_task_delay_ms(uint32_t ms);
void osal_task_yield(void);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TASK_H */
