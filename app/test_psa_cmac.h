/**
 * @file test_psa_cmac.h
 * @brief AES-128-CMAC smoke test over the PSA (TF-M) API. RFC 4493 KAT.
 */
#ifndef APP_TEST_PSA_CMAC_H
#define APP_TEST_PSA_CMAC_H

#include <stdint.h>

/**
 * Calls psa_crypto_init(), runs the RFC 4493 CMAC known-answer vectors,
 * logs "CMAC  PASS" / "CMAC  FAIL" plus per-step detail.
 * @return 0 on success, negative on any mismatch.
 */
int32_t test_psa_cmac(void);

#endif /* APP_TEST_PSA_CMAC_H */
