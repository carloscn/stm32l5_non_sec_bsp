/**
 * @file osal_tfm.c
 * @brief TF-M NSPE glue (see osal_tfm.h). Built only in the TF-M NS build.
 *
 * Two responsibilities:
 *   1. osal_tfm_ns_init() -> tfm_ns_interface_init()
 *   2. define the os_wrapper mutex hooks that tfm_ns_interface_rtos.c calls,
 *      implemented on osal_mutex_* (no direct FreeRTOS here).
 */
#include "osal_tfm.h"
#include "osal_mutex.h"

#include "tfm_ns_interface.h"
#include "os_wrapper/mutex.h"

osal_err_t osal_tfm_ns_init(void)
{
    return (tfm_ns_interface_init() == 0U) ? OSAL_OK : OSAL_ERR_INTERNAL;
}

/* ---- os_wrapper/mutex.h: required by tfm_ns_interface_rtos.c ------------- */

void *os_wrapper_mutex_create(void)
{
    return (void *)osal_mutex_create();
}

uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout)
{
    uint32_t ms = (timeout == OS_WRAPPER_WAIT_FOREVER) ? OSAL_WAIT_FOREVER : timeout;

    return OSAL_IS_OK(osal_mutex_take((osal_mutex_t)handle, ms))
           ? OS_WRAPPER_SUCCESS : OS_WRAPPER_ERROR;
}

uint32_t os_wrapper_mutex_release(void *handle)
{
    return OSAL_IS_OK(osal_mutex_give((osal_mutex_t)handle))
           ? OS_WRAPPER_SUCCESS : OS_WRAPPER_ERROR;
}

uint32_t os_wrapper_mutex_delete(void *handle)
{
    osal_mutex_destroy((osal_mutex_t)handle);
    return OS_WRAPPER_SUCCESS;
}
