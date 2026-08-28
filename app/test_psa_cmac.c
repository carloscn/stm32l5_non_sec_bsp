/**
 * @file test_psa_cmac.c
 * @brief AES-128-CMAC known-answer test over the PSA API (TF-M Crypto partition).
 *
 * Vectors: RFC 4493 (identical to NIST SP 800-38B).
 *   K  = 2b7e1516 28aed2a6 abf71588 09cf4f3c
 *   Example 2: M (16 B)  -> T = 070a16b4 6b4d4144 f79bdd9d d04a287c
 *   Example 4: M (64 B)  -> T = 51f0bebf 7e3b9d92 fc497417 79363cfe
 */
#include "crypto_smoketest.h"
#include "osal_log.h"
#include "psa/crypto.h"
#include <string.h>

static const uint8_t k_key[16] = {
    0x2b,0x7e,0x15,0x16, 0x28,0xae,0xd2,0xa6, 0xab,0xf7,0x15,0x88, 0x09,0xcf,0x4f,0x3c
};

/* RFC 4493 message block(s). Example 4 uses the full 64 bytes; example 2 the first 16. */
static const uint8_t k_msg[64] = {
    0x6b,0xc1,0xbe,0xe2, 0x2e,0x40,0x9f,0x96, 0xe9,0x3d,0x7e,0x11, 0x73,0x93,0x17,0x2a,
    0xae,0x2d,0x8a,0x57, 0x1e,0x03,0xac,0x9c, 0x9e,0xb7,0x6f,0xac, 0x45,0xaf,0x8e,0x51,
    0x30,0xc8,0x1c,0x46, 0xa3,0x5c,0xe4,0x11, 0xe5,0xfb,0xc1,0x19, 0x1a,0x0a,0x52,0xef,
    0xf6,0x9f,0x24,0x45, 0xdf,0x4f,0x9b,0x17, 0xad,0x2b,0x41,0x7b, 0xe6,0x6c,0x37,0x10
};

static const uint8_t k_tag_ex2[16] = {
    0x07,0x0a,0x16,0xb4, 0x6b,0x4d,0x41,0x44, 0xf7,0x9b,0xdd,0x9d, 0xd0,0x4a,0x28,0x7c
};
static const uint8_t k_tag_ex4[16] = {
    0x51,0xf0,0xbe,0xbf, 0x7e,0x3b,0x9d,0x92, 0xfc,0x49,0x74,0x17, 0x79,0x36,0x3c,0xfe
};

static int32_t cmac_one(psa_key_id_t key, const uint8_t *msg, size_t msg_len,
                        const uint8_t exp[16], const char *label)
{
    uint8_t  mac[16];
    size_t   mac_len = 0U;
    psa_status_t s = psa_mac_compute(key, PSA_ALG_CMAC, msg, msg_len,
                                     mac, sizeof(mac), &mac_len);
    if (s != PSA_SUCCESS) {
        osal_log_printf("  CMAC %s: psa_mac_compute = %ld", label, (long)s);
        return -1;
    }
    if (mac_len != 16U || memcmp(mac, exp, 16) != 0) {
        osal_log_printf("  CMAC %s: MISMATCH (len=%u)", label, (unsigned)mac_len);
        return -1;
    }
    osal_log_printf("  CMAC %s: ok", label);
    return 0;
}

int32_t test_psa_cmac(void)
{
    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = PSA_KEY_ID_NULL;
    int32_t rc = 0;

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&attr, PSA_ALG_CMAC);
    psa_set_key_type(&attr, PSA_KEY_TYPE_AES);

    psa_status_t s = psa_import_key(&attr, k_key, sizeof(k_key), &key);
    if (s != PSA_SUCCESS) {
        osal_log_printf("  CMAC: psa_import_key = %ld", (long)s);
        return -1;
    }

    if (cmac_one(key, k_msg, 16U, k_tag_ex2, "rfc4493-ex2") != 0) rc = -1;
    if (cmac_one(key, k_msg, 64U, k_tag_ex4, "rfc4493-ex4") != 0) rc = -1;

    (void)psa_destroy_key(key);
    return rc;
}
