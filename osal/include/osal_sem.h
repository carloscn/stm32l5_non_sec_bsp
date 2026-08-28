/**
 * @file osal_sem.h
 * @brief Opaque semaphore.
 */
#ifndef OSAL_SEM_H
#define OSAL_SEM_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct osal_sem *osal_sem_t;

osal_sem_t osal_sem_create_binary(void);
osal_sem_t osal_sem_create_counting(uint32_t max_count, uint32_t initial);
void osal_sem_destroy(osal_sem_t s);

osal_err_t osal_sem_take(osal_sem_t s, uint32_t timeout_ms);
osal_err_t osal_sem_give(osal_sem_t s);

/**
 * Give from ISR. @param higher_prio_woken may be NULL; if non-NULL set to true
 * when a higher-priority task was woken (caller should yield).
 */
osal_err_t osal_sem_give_from_isr(osal_sem_t s, bool *higher_prio_woken);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_SEM_H */
