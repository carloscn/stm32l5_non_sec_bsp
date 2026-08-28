/**
 * @file crypto_smoketest.h
 * @brief PSA (TF-M) crypto known-answer smoke tests for board bring-up.
 *
 * All access is through psa/crypto.h -> TF-M Crypto partition (via psa_call).
 * Run once from the NS entry point before the scheduler starts.
 */
#ifndef APP_CRYPTO_SMOKETEST_H
#define APP_CRYPTO_SMOKETEST_H

#include <stdint.h>

/** AES-128-CMAC KAT, RFC 4493 (examples 2 and 4). 0 = pass. */
int32_t test_psa_cmac(void);

/** Ed25519 KAT, RFC 8032 section 7.1 test 1 (sign + verify + negative). 0 = pass. */
int32_t test_psa_ed25519(void);

/**
 * Run every crypto KAT, logging PASS/FAIL per test.
 * @return 0 if all passed, else the count of failed tests (negative).
 */
int32_t crypto_smoketest_run(void);

#endif /* APP_CRYPTO_SMOKETEST_H */
