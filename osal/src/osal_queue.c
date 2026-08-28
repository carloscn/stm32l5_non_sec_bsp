/**
 * @file osal_queue.c
 */
#include "osal_queue.h"
#include "FreeRTOS.h"
#include "queue.h"

struct osal_queue {
    QueueHandle_t h;
};

static TickType_t ms_to_ticks(uint32_t timeout_ms)
{
    if (OSAL_WAIT_FOREVER == timeout_ms) {
        return portMAX_DELAY;
    }
    if (OSAL_WAIT_NONE == timeout_ms) {
        return 0;
    }
    return pdMS_TO_TICKS(timeout_ms);
}

osal_queue_t osal_queue_create(uint32_t length, uint32_t item_size)
{
    osal_queue_t q = (osal_queue_t)pvPortMalloc(sizeof(*q));
    if (NULL == q) {
        return NULL;
    }
    q->h = xQueueCreate((UBaseType_t)length, (UBaseType_t)item_size);
    if (NULL == q->h) {
        vPortFree(q);
        return NULL;
    }
    return q;
}

void osal_queue_destroy(osal_queue_t q)
{
    if (NULL == q) {
        return;
    }
    if (NULL != q->h) {
        vQueueDelete(q->h);
    }
    vPortFree(q);
}

osal_err_t osal_queue_send(osal_queue_t q, const void *item, uint32_t timeout_ms)
{
    if ((NULL == q) || (NULL == q->h) || (NULL == item)) {
        return OSAL_ERR_PARAM;
    }
    if (pdTRUE != xQueueSend(q->h, item, ms_to_ticks(timeout_ms))) {
        return OSAL_ERR_TIMEOUT;
    }
    return OSAL_OK;
}

osal_err_t osal_queue_receive(osal_queue_t q, void *item, uint32_t timeout_ms)
{
    if ((NULL == q) || (NULL == q->h) || (NULL == item)) {
        return OSAL_ERR_PARAM;
    }
    if (pdTRUE != xQueueReceive(q->h, item, ms_to_ticks(timeout_ms))) {
        return OSAL_ERR_TIMEOUT;
    }
    return OSAL_OK;
}

osal_err_t osal_queue_send_from_isr(osal_queue_t q, const void *item,
                                    bool *higher_prio_woken)
{
    BaseType_t woken = pdFALSE;

    if ((NULL == q) || (NULL == q->h) || (NULL == item)) {
        return OSAL_ERR_PARAM;
    }
    if (pdTRUE != xQueueSendFromISR(q->h, item, &woken)) {
        return OSAL_ERR;
    }
    if (NULL != higher_prio_woken) {
        *higher_prio_woken = (pdTRUE == woken);
    }
    return OSAL_OK;
}
