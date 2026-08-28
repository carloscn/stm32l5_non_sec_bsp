/**
 * @file ns/os_wrapper_freertos.c
 * @brief os_wrapper/mutex.h implementation for TF-M's tfm_ns_interface_rtos.c,
 *        backed by FreeRTOS. Makes NS->SPE PSA calls thread-safe.
 */
#include "os_wrapper/mutex.h"
#include "FreeRTOS.h"
#include "semphr.h"

void *os_wrapper_mutex_create(void)
{
    return (void *)xSemaphoreCreateMutex();
}

uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout)
{
    if (handle == NULL) {
        return OS_WRAPPER_ERROR;
    }
    TickType_t ticks = (timeout == OS_WRAPPER_WAIT_FOREVER)
                       ? portMAX_DELAY : pdMS_TO_TICKS(timeout);
    return (xSemaphoreTake((SemaphoreHandle_t)handle, ticks) == pdTRUE)
           ? OS_WRAPPER_SUCCESS : OS_WRAPPER_ERROR;
}

uint32_t os_wrapper_mutex_release(void *handle)
{
    if (handle == NULL) {
        return OS_WRAPPER_ERROR;
    }
    return (xSemaphoreGive((SemaphoreHandle_t)handle) == pdTRUE)
           ? OS_WRAPPER_SUCCESS : OS_WRAPPER_ERROR;
}

uint32_t os_wrapper_mutex_delete(void *handle)
{
    if (handle != NULL) {
        vSemaphoreDelete((SemaphoreHandle_t)handle);
    }
    return OS_WRAPPER_SUCCESS;
}
