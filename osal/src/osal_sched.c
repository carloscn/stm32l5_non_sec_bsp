/**
 * @file osal_sched.c
 */
#include "osal_sched.h"
#include "FreeRTOS.h"
#include "task.h"

bool osal_sched_is_running(void)
{
    return (taskSCHEDULER_RUNNING == xTaskGetSchedulerState());
}

void osal_sched_critical_enter(void)
{
    taskENTER_CRITICAL();
}

void osal_sched_critical_exit(void)
{
    taskEXIT_CRITICAL();
}

void osal_sched_yield_from_isr(bool higher_prio_woken)
{
    BaseType_t woken = higher_prio_woken ? pdTRUE : pdFALSE;
    portYIELD_FROM_ISR(woken);
}

void osal_sched_start(void)
{
    vTaskStartScheduler();
}

uint32_t osal_time_get_ms(void)
{
    if (!osal_sched_is_running()) {
        return 0U;
    }
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}
