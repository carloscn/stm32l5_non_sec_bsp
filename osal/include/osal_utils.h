
#ifndef OSAL_UTILS_H_
#define OSAL_UTILS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Convert a uint8_t array to a hexadecimal string.
 * Each byte is formatted as two uppercase hex digits with a space after each.
 * The output string is null-terminated and does not include a trailing space.
 * @param data: Input byte array.
 * @param length: Length of the input array.
 * @param output: Buffer to store the hex string.
 * @param output_size: Size of the output buffer.
 * @return: Number of bytes written to output (excluding null terminator), or 0 if buffer is too small.
 */
size_t osal_utils_uint8_array_to_hex(const uint8_t *data, size_t length, char *output, size_t output_size);

/**
 * Delay for a specified number of microseconds using a loop.
 * Tuned for S32K312 with a 120 MHz core clock (120 cycles per microsecond).
 * Uses inline assembly for predictable cycle counts.
 * @param us: Number of microseconds to delay.
 */
void osal_utils_delay_us(size_t us);

/**
 * Delay for a specified number of milliseconds by calling osal_delay_us.
 * @param ms: Number of milliseconds to delay.
 */
void osal_utils_delay_ms(size_t ms);

/** Byte length for little-endian uint32_t wire format (e.g. mbedTLS ALT key handles). */
#define OSAL_UTILS_U32_LE_BYTES  (4U)

/**
 * Encode uint32 as little-endian bytes.
 * Use instead of mbedtls_mpi_read_binary (big-endian) for NXP ALT handle paths.
 */
void osal_utils_u32_to_le_bytes(uint32_t value, uint8_t out[OSAL_UTILS_U32_LE_BYTES]);

/** Decode little-endian bytes to uint32. */
uint32_t osal_utils_u32_from_le_bytes(const uint8_t in[OSAL_UTILS_U32_LE_BYTES]);

/** Read uint32 from first little-endian machine word (e.g. mbedtls_mpi limb after genkey). */
uint32_t osal_utils_u32_from_le_word(const uint32_t *word);

#endif /* OSAL_UTILS_H_ */
