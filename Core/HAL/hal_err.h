#ifndef HAL_ERR_H
#define HAL_ERR_H

#define HAL_SUCCESS                0   // Operation completed successfully
#define HAL_ERR                   -1  // General error
#define HAL_ERR_TIMEOUT           -2  // Operation timed out
#define HAL_ERR_NULL_POINTER      -3  // Null pointer error
#define HAL_ERR_INVALID_PARAM     -4  // Invalid parameter
#define HAL_ERR_OUT_OF_MEMORY     -5  // Memory allocation failure
#define HAL_ERR_NOT_INITIALIZED   -6  // Resource not initialized
#define HAL_ERR_BUSY              -7  // Resource is busy or in use
#define HAL_ERR_UNSUPPORTED       -8  // Operation not supported
#define HAL_ERR_COMMUNICATION     -9  // Communication failure (e.g., I2C, SPI)
#define HAL_ERR_PERMISSION        -10 // Permission denied (e.g., write-protected)
#define HAL_ERR_OVERFLOW          -11 // Data overflow or buffer overflow
#define HAL_ERR_UNDERFLOW         -12 // Data underflow or buffer underflow
#define HAL_ERR_HARDWARE_FAILURE  -13 // Hardware failure or malfunction
#define HAL_ERR_CRC               -14 // CRC (Cyclic Redundancy Check) error
#define HAL_ERR_FLASH_WRITE       -15 // Flash write error
#define HAL_ERR_FLASH_READ        -16 // Flash read error
#define HAL_ERR_FLASH_ERASE       -17 // Flash erase error
#define HAL_ERR_HW_READ           -18
#define HAL_ERR_HW_WRITE          -19
#define HAL_ERR_HW_CONTROL        -20
#define HAL_ERR_HW_LACK           -21
#define HAL_ERR_MALLOC            -22
#define HAL_ERR_INIT              -23
#define HAL_ERR_HW_STATE          -24
#define HAL_ERR_HW_RECV           -25
#define HAL_ERR_HW_SEND           -26


#endif // HAL_ERR_H