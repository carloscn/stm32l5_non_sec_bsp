/**
 * @file osal_sem.c
 */
#include "osal_sem.h"
#include "FreeRTOS.h"
#include "semphr.h"

struct osal_sem {
    SemaphoreHandle_t h;
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

osal_sem_t osal_sem_create_binary(void)
{
    osal_sem_t s = (osal_sem_t)pvPortMalloc(sizeof(*s));
    if (NULL == s) {
        return NULL;
    }
    s->h = xSemaphoreCreateBinary();
    if (NULL == s->h) {
        vPortFree(s);
        return NULL;
    }
    return s;
}

osal_sem_t osal_sem_create_counting(uint32_t max_count, uint32_t initial)
{
    osal_sem_t s = (osal_sem_t)pvPortMalloc(sizeof(*s));
    if (NULL == s) {
        return NULL;
    }
    s->h = xSemaphoreCreateCounting((UBaseType_t)max_count, (UBaseType_t)initial);
    if (NULL == s->h) {
        vPortFree(s);
        return NULL;
    }
    return s;
}

void osal_sem_destroy(osal_sem_t s)
{
    if (NULL == s) {
        return;
    }
    if (NULL != s->h) {
        vSemaphoreDelete(s->h);
    }
    vPortFree(s);
}

osal_err_t osal_sem_take(osal_sem_t s, uint32_t timeout_ms)
{
    if ((NULL == s) || (NULL == s->h)) {
        return OSAL_ERR_INVALID_PARAM;
    }
    if (pdTRUE != xSemaphoreTake(s->h, ms_to_ticks(timeout_ms))) {
        return OSAL_ERR_TIMEOUT;
    }
    return OSAL_OK;
}

osal_err_t osal_sem_give(osal_sem_t s)
{
    if ((NULL == s) || (NULL == s->h)) {
        return OSAL_ERR_INVALID_PARAM;
    }
    if (pdTRUE != xSemaphoreGive(s->h)) {
        return OSAL_ERR_SEM_OVERFLOW;
    }
    return OSAL_OK;
}

osal_err_t osal_sem_give_from_isr(osal_sem_t s, bool *higher_prio_woken)
{
    BaseType_t woken = pdFALSE;

    if ((NULL == s) || (NULL == s->h)) {
        return OSAL_ERR_INVALID_PARAM;
    }
    if (pdTRUE != xSemaphoreGiveFromISR(s->h, &woken)) {
        return OSAL_ERR_SEM_OVERFLOW;
    }
    if (NULL != higher_prio_woken) {
        *higher_prio_woken = (pdTRUE == woken);
    }
    return OSAL_OK;
}
