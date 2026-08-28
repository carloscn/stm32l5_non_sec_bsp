/**
 * @file heartbeat.c
 * @brief Heartbeat log task. Platform access via osal only.
 */
#include "heartbeat.h"
#include "osal_task.h"
#include "osal_log.h"
#include "osal_heap.h"

#define HB_PERIOD_MS    (1000U)
#define HB_STACK_WORDS  (256U)
#define HB_PRIORITY     (2U)

static void heartbeat_task(void *arg)
{
    uint32_t n = 0U;
    (void)arg;
    for (;;) {
        osal_log_printf("heartbeat %lu  (heap free %u)",
                        (unsigned long)n++, (unsigned)osal_heap_get_free());
        osal_task_delay_ms(HB_PERIOD_MS);
    }
}

void heartbeat_start(void)
{
    if (osal_task_create(heartbeat_task, "hb", HB_STACK_WORDS, NULL,
                         HB_PRIORITY, NULL) != OSAL_OK) {
        osal_log_info("heartbeat: task create failed");
    }
}
