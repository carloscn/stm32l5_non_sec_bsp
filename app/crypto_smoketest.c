/**
 * @file crypto_smoketest.c
 * @brief Aggregates the PSA (TF-M) crypto known-answer tests.
 */
#include "crypto_smoketest.h"
#include "osal_log.h"
#include "psa/crypto.h"

int32_t crypto_smoketest_run(void)
{
    int32_t failed = 0;

    psa_status_t s = psa_crypto_init();
    if (s != PSA_SUCCESS) {
        osal_log_printf("crypto_smoketest: psa_crypto_init = %ld", (long)s);
        return -1;
    }

    osal_log_info("crypto_smoketest: AES-128-CMAC (RFC 4493)");
    if (test_psa_cmac() != 0) {
        osal_log_info("crypto_smoketest: CMAC   FAIL");
        failed++;
    } else {
        osal_log_info("crypto_smoketest: CMAC   PASS");
    }

    osal_log_info("crypto_smoketest: ECDSA P-256 (RFC 6979 KAT + round trip)");
    if (test_psa_ecdsa_p256() != 0) {
        osal_log_info("crypto_smoketest: ECDSA  FAIL");
        failed++;
    } else {
        osal_log_info("crypto_smoketest: ECDSA  PASS");
    }

    osal_log_printf("crypto_smoketest: %s (%ld failed)",
                    (failed == 0) ? "ALL PASS" : "FAILURES", (long)failed);
    return -failed;
}
