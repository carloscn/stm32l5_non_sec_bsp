/**
 * @file osal_heap.c
 */
#include "osal_heap.h"
#include "FreeRTOS.h"
#include <string.h>
#include <stdint.h>

void *osal_malloc(size_t size)
{
    return pvPortMalloc(size);
}

void *osal_calloc(size_t nmemb, size_t size)
{
    size_t total;
    void *p;

    if ((0U == nmemb) || (0U == size)) {
        return NULL;
    }
    if (nmemb > ((size_t)UINT32_MAX / size)) {
        return NULL;
    }
    total = nmemb * size;
    p = pvPortMalloc(total);
    if (NULL != p) {
        (void)memset(p, 0, total);
    }
    return p;
}

void osal_free(void *ptr)
{
    vPortFree(ptr);
}

size_t osal_heap_get_free(void)
{
    return (size_t)xPortGetFreeHeapSize();
}
