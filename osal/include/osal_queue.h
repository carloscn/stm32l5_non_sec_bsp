/**
 * @file osal_queue.h
 * @brief Opaque message queue.
 */
#ifndef OSAL_QUEUE_H
#define OSAL_QUEUE_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct osal_queue *osal_queue_t;

osal_queue_t osal_queue_create(uint32_t length, uint32_t item_size);
void osal_queue_destroy(osal_queue_t q);

osal_err_t osal_queue_send(osal_queue_t q, const void *item, uint32_t timeout_ms);
osal_err_t osal_queue_receive(osal_queue_t q, void *item, uint32_t timeout_ms);

/**
 * Send from ISR. @param higher_prio_woken may be NULL.
 */
osal_err_t osal_queue_send_from_isr(osal_queue_t q, const void *item,
                                    bool *higher_prio_woken);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_QUEUE_H */
