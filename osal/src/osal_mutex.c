/**
 * @file osal_mutex.c
 */
#include "osal_mutex.h"
#include "osal_sched.h"
#include "FreeRTOS.h"
#include "semphr.h"

struct osal_mutex {
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

osal_mutex_t osal_mutex_create(void)
{
    osal_mutex_t m = (osal_mutex_t)pvPortMalloc(sizeof(*m));
    if (NULL == m) {
        return NULL;
    }
    m->h = xSemaphoreCreateMutex();
    if (NULL == m->h) {
        vPortFree(m);
        return NULL;
    }
    return m;
}

void osal_mutex_destroy(osal_mutex_t m)
{
    if (NULL == m) {
        return;
    }
    if (NULL != m->h) {
        vSemaphoreDelete(m->h);
    }
    vPortFree(m);
}

osal_err_t osal_mutex_take(osal_mutex_t m, uint32_t timeout_ms)
{
    if ((NULL == m) || (NULL == m->h)) {
        return OSAL_ERR_INVALID_PARAM;
    }
    if (!osal_sched_is_running()) {
        return OSAL_OK;
    }
    if (pdTRUE != xSemaphoreTake(m->h, ms_to_ticks(timeout_ms))) {
        return OSAL_ERR_TIMEOUT;
    }
    return OSAL_OK;
}

osal_err_t osal_mutex_give(osal_mutex_t m)
{
    if ((NULL == m) || (NULL == m->h)) {
        return OSAL_ERR_INVALID_PARAM;
    }
    if (!osal_sched_is_running()) {
        return OSAL_OK;
    }
    if (pdTRUE != xSemaphoreGive(m->h)) {
        return OSAL_ERR_MUTEX_NOT_OWNED;
    }
    return OSAL_OK;
}
