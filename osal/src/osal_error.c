/**
 * @file osal_error.c
 * @brief osal_err_str() — keep names in sync with osal_error.h.
 */
#include "osal_error.h"

const char *osal_err_str(osal_err_t err)
{
    switch (err) {
    case OSAL_ERR_SUCCESS:              return "OK";
    case OSAL_ERR_INVALID_PARAM:        return "INVALID_PARAM";
    case OSAL_ERR_NOT_INITIALIZED:      return "NOT_INITIALIZED";
    case OSAL_ERR_TIMEOUT:              return "TIMEOUT";
    case OSAL_ERR_RESOURCE_BUSY:        return "RESOURCE_BUSY";
    case OSAL_ERR_NOT_SUPPORTED:        return "NOT_SUPPORTED";
    case OSAL_ERR_OUT_OF_MEMORY:        return "OUT_OF_MEMORY";
    case OSAL_ERR_PERMISSION_DENIED:    return "PERMISSION_DENIED";
    case OSAL_ERR_INTERNAL:             return "INTERNAL";
    case OSAL_ERR_FULL:                 return "FULL";
    case OSAL_ERR_EMPTY:                return "EMPTY";
    case OSAL_ERR_ISR_CONTEXT:          return "ISR_CONTEXT";
    case OSAL_ERR_SCHED_NOT_RUNNING:    return "SCHED_NOT_RUNNING";
    case OSAL_ERR_ALREADY_EXISTS:       return "ALREADY_EXISTS";
    case OSAL_ERR_NOT_FOUND:            return "NOT_FOUND";

    case OSAL_ERR_MUTEX_CREATE_FAILED:  return "MUTEX_CREATE_FAILED";
    case OSAL_ERR_MUTEX_TAKE_FAILED:    return "MUTEX_TAKE_FAILED";
    case OSAL_ERR_MUTEX_GIVE_FAILED:    return "MUTEX_GIVE_FAILED";
    case OSAL_ERR_MUTEX_NOT_OWNED:      return "MUTEX_NOT_OWNED";

    case OSAL_ERR_SEM_CREATE_FAILED:    return "SEM_CREATE_FAILED";
    case OSAL_ERR_SEM_TAKE_FAILED:      return "SEM_TAKE_FAILED";
    case OSAL_ERR_SEM_GIVE_FAILED:      return "SEM_GIVE_FAILED";
    case OSAL_ERR_SEM_OVERFLOW:         return "SEM_OVERFLOW";

    case OSAL_ERR_QUEUE_CREATE_FAILED:  return "QUEUE_CREATE_FAILED";
    case OSAL_ERR_QUEUE_SEND_FAILED:    return "QUEUE_SEND_FAILED";
    case OSAL_ERR_QUEUE_RECV_FAILED:    return "QUEUE_RECV_FAILED";
    case OSAL_ERR_QUEUE_FULL:           return "QUEUE_FULL";
    case OSAL_ERR_QUEUE_EMPTY:          return "QUEUE_EMPTY";

    case OSAL_ERR_TASK_CREATE_FAILED:   return "TASK_CREATE_FAILED";
    case OSAL_ERR_TASK_DELETE_FAILED:   return "TASK_DELETE_FAILED";
    case OSAL_ERR_TASK_INVALID_PRIORITY:return "TASK_INVALID_PRIORITY";
    case OSAL_ERR_TASK_STACK_TOO_SMALL: return "TASK_STACK_TOO_SMALL";

    case OSAL_ERR_HEAP_ALLOC_FAILED:    return "HEAP_ALLOC_FAILED";
    case OSAL_ERR_HEAP_INVALID_PTR:     return "HEAP_INVALID_PTR";

    case OSAL_ERR_LOG_NOT_READY:        return "LOG_NOT_READY";
    case OSAL_ERR_LOG_QUEUE_FULL:       return "LOG_QUEUE_FULL";

    case OSAL_ERR_SCHED_START_FAILED:   return "SCHED_START_FAILED";

    default:                            return "UNKNOWN";
    }
}
