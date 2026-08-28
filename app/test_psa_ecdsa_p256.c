/**
 * @file test_psa_ecdsa_p256.c
 * @brief ECDSA P-256 smoke test over the PSA API (TF-M Crypto partition).
 *
 * mbed-crypto (TF-M's PSA backend) has no EdDSA/Ed25519 implementation, so the
 * signature smoke uses ECDSA secp256r1 + SHA-256 - the same primitive as the
 * reference S32K312 project's test_ecdsa.c.
 *
 * Known-answer verify vector: RFC 6979 A.2.5 (P-256, SHA-256, message "sample").
 *   Ux/Uy = public point;  r/s = signature over SHA-256("sample").
 * Plus a generate -> sign -> verify round trip and a negative check.
 */
#include "crypto_smoketest.h"
#include "osal_log.h"
#include "psa/crypto.h"
#include <string.h>

/* RFC 6979 A.2.5 public key U (secp256r1). */
static const uint8_t k_pub_xy[64] = {
    /* Ux */
    0x60,0xFE,0xD4,0xBA, 0x25,0x5A,0x9D,0x31, 0xC9,0x61,0xEB,0x74, 0xC6,0x35,0x6D,0x68,
    0xC0,0x49,0xB8,0x92, 0x3B,0x61,0xFA,0x6C, 0xE6,0x69,0x62,0x2E, 0x60,0xF2,0x9F,0xB6,
    /* Uy */
    0x79,0x03,0xFE,0x10, 0x08,0xB8,0xBC,0x99, 0xA4,0x1A,0xE9,0xE9, 0x56,0x28,0xBC,0x64,
    0xF2,0xF1,0xB2,0x0C, 0x2D,0x7E,0x9F,0x51, 0x77,0xA3,0xC2,0x94, 0xD4,0x46,0x22,0x99
};

/* Signature over SHA-256("sample"), RFC 6979 A.2.5: r || s. */
static const uint8_t k_sig_rs[64] = {
    /* r */
    0xEF,0xD4,0x8B,0x2A, 0xAC,0xB6,0xA8,0xFD, 0x11,0x40,0xDD,0x9C, 0xD4,0x5E,0x81,0xD6,
    0x9D,0x2C,0x87,0x7B, 0x56,0xAA,0xF9,0x91, 0xC3,0x4D,0x0E,0xA8, 0x4E,0xAF,0x37,0x16,
    /* s */
    0xF7,0xCB,0x1C,0x94, 0x2D,0x65,0x7C,0x41, 0xD4,0x36,0xC7,0xA1, 0xB6,0xE2,0x9F,0x65,
    0xF3,0xE9,0x00,0xDB, 0xB9,0xAF,0xF4,0x06, 0x4D,0xC4,0xAB,0x2F, 0x84,0x3A,0xCD,0xA8
};

static const uint8_t k_msg[6] = { 's','a','m','p','l','e' };

#define ALG   PSA_ALG_ECDSA(PSA_ALG_SHA_256)

static int32_t known_answer_verify(void)
{
    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t pub = PSA_KEY_ID_NULL;
    uint8_t point[65];
    int32_t rc = 0;

    point[0] = 0x04;                            /* uncompressed */
    memcpy(&point[1], k_pub_xy, 64);

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attr, ALG);
    psa_set_key_type(&attr, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));

    psa_status_t s = psa_import_key(&attr, point, sizeof(point), &pub);
    if (s != PSA_SUCCESS) {
        osal_log_printf("  ECDSA KAT: import pubkey = %ld", (long)s);
        return -1;
    }

    s = psa_verify_message(pub, ALG, k_msg, sizeof(k_msg), k_sig_rs, sizeof(k_sig_rs));
    if (s != PSA_SUCCESS) {
        osal_log_printf("  ECDSA KAT: verify(good) = %ld", (long)s);
        rc = -1;
    } else {
        osal_log_info("  ECDSA KAT: verify(good) ok");
    }

    uint8_t bad[64];
    memcpy(bad, k_sig_rs, 64);
    bad[0] ^= 0x01;
    s = psa_verify_message(pub, ALG, k_msg, sizeof(k_msg), bad, sizeof(bad));
    if (s != PSA_ERROR_INVALID_SIGNATURE) {
        osal_log_printf("  ECDSA KAT: verify(bad) = %ld (want INVALID_SIGNATURE)", (long)s);
        rc = -1;
    } else {
        osal_log_info("  ECDSA KAT: verify(bad) rejected ok");
    }

    (void)psa_destroy_key(pub);
    return rc;
}

static int32_t sign_verify_roundtrip(void)
{
    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = PSA_KEY_ID_NULL;
    uint8_t sig[PSA_SIGNATURE_MAX_SIZE];
    size_t  sig_len = 0U;
    int32_t rc = 0;

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attr, ALG);
    psa_set_key_type(&attr, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
    psa_set_key_bits(&attr, 256);

    psa_status_t s = psa_generate_key(&attr, &key);
    if (s != PSA_SUCCESS) {
        osal_log_printf("  ECDSA rt: generate_key = %ld", (long)s);
        return -1;
    }

    s = psa_sign_message(key, ALG, k_msg, sizeof(k_msg), sig, sizeof(sig), &sig_len);
    if (s != PSA_SUCCESS) {
        osal_log_printf("  ECDSA rt: sign = %ld", (long)s);
        rc = -1;
    } else {
        s = psa_verify_message(key, ALG, k_msg, sizeof(k_msg), sig, sig_len);
        if (s != PSA_SUCCESS) {
            osal_log_printf("  ECDSA rt: verify = %ld", (long)s);
            rc = -1;
        } else {
            osal_log_info("  ECDSA rt: gen/sign/verify ok");
        }
    }

    (void)psa_destroy_key(key);
    return rc;
}

int32_t test_psa_ecdsa_p256(void)
{
    int32_t rc = 0;
    if (known_answer_verify() != 0)  rc = -1;
    if (sign_verify_roundtrip() != 0) rc = -1;
    return rc;
}
