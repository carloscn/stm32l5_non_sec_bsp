/**
 * @file heartbeat.c
 * @brief Heartbeat log task body. No task creation here (see main()).
 */
#include "heartbeat.h"
#include "osal_task.h"   /* osal_task_delay_ms */
#include "osal_log.h"
#include "osal_heap.h"

#define HB_PERIOD_MS    (1000U)

void heartbeat_task(void *arg)
{
    uint32_t n = 0U;
    (void)arg;
    for (;;) {
        osal_log_printf("heartbeat %lu  (heap free %u)",
                        (unsigned long)n++, (unsigned)osal_heap_get_free());
        osal_task_delay_ms(HB_PERIOD_MS);
    }
}
