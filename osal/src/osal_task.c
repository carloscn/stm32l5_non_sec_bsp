/**
 * @file osal_task.c
 */
#include "osal_task.h"
#include "FreeRTOS.h"
#include "task.h"

struct osal_task {
    TaskHandle_t h;
};

osal_err_t osal_task_create(osal_task_fn_t fn, const char *name,
                            uint32_t stack_words, void *arg,
                            uint32_t priority, osal_task_t *out_handle)
{
    TaskHandle_t th = NULL;
    BaseType_t ok;

    if ((NULL == fn) || (0U == stack_words)) {
        return OSAL_ERR_INVALID_PARAM;
    }

    ok = xTaskCreate((TaskFunction_t)fn, name, (configSTACK_DEPTH_TYPE)stack_words,
                     arg, (UBaseType_t)priority, &th);
    if (pdPASS != ok) {
        return OSAL_ERR_TASK_CREATE_FAILED;
    }

    if (NULL != out_handle) {
        osal_task_t t = (osal_task_t)pvPortMalloc(sizeof(*t));
        if (NULL == t) {
            vTaskDelete(th);
            return OSAL_ERR_OUT_OF_MEMORY;
        }
        t->h = th;
        *out_handle = t;
    }

    return OSAL_OK;
}

void osal_task_delete(osal_task_t t)
{
    TaskHandle_t th;

    if (NULL == t) {
        /* Delete self. */
        vTaskDelete(NULL);
        return;
    }
    th = t->h;
    vPortFree(t);
    vTaskDelete(th);
}

void osal_task_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void osal_task_yield(void)
{
    taskYIELD();
}
