/**
 * @file osal_hooks.c
 * @brief FreeRTOS application hook functions, kept out of application code.
 *
 * Enabled by configUSE_MALLOC_FAILED_HOOK / configCHECK_FOR_STACK_OVERFLOW
 * in FreeRTOSConfig.h. Both route to osal_panic().
 */
#include "osal_sched.h"
#include "osal_log.h"
#include "FreeRTOS.h"
#include "task.h"

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
void vApplicationMallocFailedHook(void)
{
    osal_panic("pvPortMalloc failed (configTOTAL_HEAP_SIZE too small)");
}
#endif

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
    (void)task;
    osal_log_info("stack overflow in task:");
    osal_panic((name != NULL) ? name : "(unknown)");
}
#endif
