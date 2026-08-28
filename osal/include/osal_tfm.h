/**
 * @file osal_tfm.h
 * @brief OSAL glue for running under Trusted Firmware-M (NSPE).
 *
 * Keeps application / app-module code free of any tfm_* or os_wrapper/*
 * dependency: they call osal_tfm_ns_init(), then psa_* directly.
 *
 * Only built in the TF-M NS build (see CMakeLists.txt). Not part of the
 * portable OSAL surface shared with the S32K312 project.
 */
#ifndef OSAL_TFM_H
#define OSAL_TFM_H

#include "osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise the TF-M non-secure interface. Must be called once, before any
 * psa_* call. Also installs the os_wrapper mutex hooks that TF-M's RTOS-aware
 * NS interface needs, backed by osal_mutex_* (so the only place a real RTOS
 * mutex is created stays osal_mutex.c).
 *
 * @return OSAL_OK or OSAL_ERR_INTERNAL.
 */
osal_err_t osal_tfm_ns_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TFM_H */
