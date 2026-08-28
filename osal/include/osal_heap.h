/**
 * @file osal_heap.h
 * @brief Heap allocation (FreeRTOS heap_4).
 */
#ifndef OSAL_HEAP_H
#define OSAL_HEAP_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *osal_malloc(size_t size);
void *osal_calloc(size_t nmemb, size_t size);
void osal_free(void *ptr);

/** Remaining FreeRTOS heap bytes (heap_4). */
size_t osal_heap_get_free(void);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_HEAP_H */
