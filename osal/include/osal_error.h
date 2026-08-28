/**
 * @file osal_error.h
 * @brief OSAL error codes (coordinated with HAL).
 *
 * Convention (same as HAL):
 *  - Success is always 0.
 *  - Failures are positive, unique codes in the OSAL range [0x6000, 0x6FFF].
 *  - HAL uses [0x7000, 0x7FFF] — ranges never overlap.
 *  - Callers test with OSAL_IS_OK() / OSAL_IS_ERR(), never raw comparisons
 *    against magic numbers other than OSAL_OK / OSAL_ERR_SUCCESS.
 *
 * Semantic pairs shared with HAL (different numeric values, same meaning):
 *  INVALID_PARAM, NOT_INITIALIZED, TIMEOUT, RESOURCE_BUSY, NOT_SUPPORTED,
 *  OUT_OF_MEMORY, INTERNAL.
 */
#ifndef OSAL_ERROR_H
#define OSAL_ERROR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t osal_err_t;

/* ------------------------------------------------------------------------- */
/* Range                                                                     */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_BASE                       (0x6000)

/* ------------------------------------------------------------------------- */
/* Success                                                                   */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_SUCCESS                    (0)
#define OSAL_OK                             OSAL_ERR_SUCCESS

/* ------------------------------------------------------------------------- */
/* General (mirror HAL general set)                                          */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_INVALID_PARAM              (OSAL_ERR_BASE + 1)   /* bad arg / NULL handle */
#define OSAL_ERR_NOT_INITIALIZED            (OSAL_ERR_BASE + 2)   /* module / object not ready */
#define OSAL_ERR_TIMEOUT                    (OSAL_ERR_BASE + 3)   /* wait expired */
#define OSAL_ERR_RESOURCE_BUSY              (OSAL_ERR_BASE + 4)   /* already held / in use */
#define OSAL_ERR_NOT_SUPPORTED              (OSAL_ERR_BASE + 5)   /* API not available */
#define OSAL_ERR_OUT_OF_MEMORY              (OSAL_ERR_BASE + 6)   /* heap / object pool exhausted */
#define OSAL_ERR_PERMISSION_DENIED          (OSAL_ERR_BASE + 7)   /* wrong context / ownership */
#define OSAL_ERR_INTERNAL                   (OSAL_ERR_BASE + 8)   /* unexpected RTOS failure */
#define OSAL_ERR_FULL                       (OSAL_ERR_BASE + 9)   /* queue / buffer full */
#define OSAL_ERR_EMPTY                      (OSAL_ERR_BASE + 10)  /* queue / buffer empty */
#define OSAL_ERR_ISR_CONTEXT                (OSAL_ERR_BASE + 11)  /* API not allowed from ISR */
#define OSAL_ERR_SCHED_NOT_RUNNING          (OSAL_ERR_BASE + 12)  /* needs running scheduler */
#define OSAL_ERR_ALREADY_EXISTS             (OSAL_ERR_BASE + 13)  /* duplicate create */
#define OSAL_ERR_NOT_FOUND                  (OSAL_ERR_BASE + 14)  /* handle / object missing */

/* ------------------------------------------------------------------------- */
/* Mutex                                                                     */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_MUTEX_CREATE_FAILED        (OSAL_ERR_BASE + 20)
#define OSAL_ERR_MUTEX_TAKE_FAILED          (OSAL_ERR_BASE + 21)
#define OSAL_ERR_MUTEX_GIVE_FAILED          (OSAL_ERR_BASE + 22)
#define OSAL_ERR_MUTEX_NOT_OWNED            (OSAL_ERR_BASE + 23)  /* give by non-owner */

/* ------------------------------------------------------------------------- */
/* Semaphore                                                                 */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_SEM_CREATE_FAILED          (OSAL_ERR_BASE + 30)
#define OSAL_ERR_SEM_TAKE_FAILED            (OSAL_ERR_BASE + 31)
#define OSAL_ERR_SEM_GIVE_FAILED            (OSAL_ERR_BASE + 32)
#define OSAL_ERR_SEM_OVERFLOW               (OSAL_ERR_BASE + 33)  /* counting max reached */

/* ------------------------------------------------------------------------- */
/* Queue                                                                     */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_QUEUE_CREATE_FAILED        (OSAL_ERR_BASE + 40)
#define OSAL_ERR_QUEUE_SEND_FAILED          (OSAL_ERR_BASE + 41)
#define OSAL_ERR_QUEUE_RECV_FAILED          (OSAL_ERR_BASE + 42)
#define OSAL_ERR_QUEUE_FULL                 (OSAL_ERR_BASE + 43)
#define OSAL_ERR_QUEUE_EMPTY                (OSAL_ERR_BASE + 44)

/* ------------------------------------------------------------------------- */
/* Task                                                                      */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_TASK_CREATE_FAILED         (OSAL_ERR_BASE + 50)
#define OSAL_ERR_TASK_DELETE_FAILED         (OSAL_ERR_BASE + 51)
#define OSAL_ERR_TASK_INVALID_PRIORITY      (OSAL_ERR_BASE + 52)
#define OSAL_ERR_TASK_STACK_TOO_SMALL       (OSAL_ERR_BASE + 53)

/* ------------------------------------------------------------------------- */
/* Heap                                                                      */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_HEAP_ALLOC_FAILED          (OSAL_ERR_BASE + 60)
#define OSAL_ERR_HEAP_INVALID_PTR           (OSAL_ERR_BASE + 61)

/* ------------------------------------------------------------------------- */
/* Log                                                                       */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_LOG_NOT_READY              (OSAL_ERR_BASE + 70)
#define OSAL_ERR_LOG_QUEUE_FULL             (OSAL_ERR_BASE + 71)

/* ------------------------------------------------------------------------- */
/* Scheduler                                                                 */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR_SCHED_START_FAILED         (OSAL_ERR_BASE + 80)

/* ------------------------------------------------------------------------- */
/* Legacy aliases (keep existing call sites compiling)                       */
/* ------------------------------------------------------------------------- */
#define OSAL_ERR                            OSAL_ERR_INTERNAL
#define OSAL_ERR_PARAM                      OSAL_ERR_INVALID_PARAM
#define OSAL_ERR_NOMEM                      OSAL_ERR_OUT_OF_MEMORY

/* ------------------------------------------------------------------------- */
/* Helpers                                                                   */
/* ------------------------------------------------------------------------- */
#define OSAL_IS_OK(err)                     ((osal_err_t)(err) == OSAL_OK)
#define OSAL_IS_ERR(err)                    ((osal_err_t)(err) != OSAL_OK)

/**
 * Human-readable name for logging / asserts.
 * Always returns a non-NULL static string.
 */
const char *osal_err_str(osal_err_t err);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_ERROR_H */
