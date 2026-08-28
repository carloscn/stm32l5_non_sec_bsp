/**
 * @file test_psa_ecdsa_p256.h
 * @brief ECDSA P-256 smoke test over the PSA (TF-M) API.
 *        RFC 6979 A.2.5 KAT verify + generate/sign/verify round trip.
 */
#ifndef APP_TEST_PSA_ECDSA_P256_H
#define APP_TEST_PSA_ECDSA_P256_H

#include <stdint.h>

/**
 * Calls psa_crypto_init(), runs the ECDSA P-256 checks, logs
 * "ECDSA P-256  PASS" / "ECDSA P-256  FAIL" plus per-step detail.
 * @return 0 on success, negative on failure.
 */
int32_t test_psa_ecdsa_p256(void);

#endif /* APP_TEST_PSA_ECDSA_P256_H */
