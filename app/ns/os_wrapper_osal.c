/**
 * @file app/ns/os_wrapper_osal.c
 * @brief os_wrapper/mutex.h for TF-M's tfm_ns_interface_rtos.c, implemented on
 *        the OSAL layer (no direct FreeRTOS calls). Makes NS->SPE PSA calls
 *        thread-safe.
 *
 * Before the scheduler starts osal_mutex_take/give are no-op success, which is
 * fine: the pre-scheduler crypto_smoketest_run() is single-threaded.
 */
#include "os_wrapper/mutex.h"
#include "osal_mutex.h"
#include "osal_types.h"

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
