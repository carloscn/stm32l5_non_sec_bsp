#-------------------------------------------------------------------------------
#  stm32l5_non_sec_bsp -- TF-M Non-Secure app (NSPE)
#
#  Consumes the SPE build export from ../stm32l5_secure_bsp/build/api_ns.
#
#  Configure:
#    SPE=../stm32l5_secure_bsp/build/api_ns
#    cmake -S . -B build_ns -G Ninja \
#          -DCONFIG_SPE_PATH=$(realpath $SPE) \
#          -DTFM_TOOLCHAIN_FILE=$(realpath $SPE)/cmake/toolchain_ns_GNUARM.cmake
#    cmake --build build_ns
#
#  The standalone (no-TF-M) image is still built with the Makefile.
#  STATUS: work in progress -- see ns/README.md.
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR
        "CONFIG_SPE_PATH ('${CONFIG_SPE_PATH}') missing. Point it at "
        "../stm32l5_secure_bsp/build/api_ns")
endif()

include(${CONFIG_SPE_PATH}/cmake/spe_config.cmake)
include(${CONFIG_SPE_PATH}/cmake/spe_export.cmake)

project("stm32l5_non_sec_bsp_ns" VERSION 0.1 LANGUAGES C ASM)

# Brings tfm_api_ns, tfm_api_ns_tz, platform_ns, platform_region_defs, and the
# NS linker script / signing custom targets.
add_subdirectory(${CONFIG_SPE_PATH} spe)

set(FRTOS   ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS)
set(CUBE    ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeL5/Drivers)

add_executable(tfm_ns
    ns/main.c
    ns/os_wrapper_freertos.c
    ${CONFIG_SPE_PATH}/interface/src/os_wrapper/tfm_ns_interface_rtos.c
    ${CONFIG_SPE_PATH}/platform/Device/Source/startup_stm32l5xx_ns.c

    app/led.c
    app/heartbeat.c
    app/crypto_smoketest.c
    app/test_psa_cmac.c
    app/test_psa_ecdsa_p256.c

    hal/src/hal_gpio.c
    hal/src/hal_uart.c
    hal/src/hal_irq.c
    hal/src/hal_mcu.c
    hal/src/hal_cache.c

    osal/src/osal_task.c
    osal/src/osal_mutex.c
    osal/src/osal_sem.c
    osal/src/osal_queue.c
    osal/src/osal_sched.c
    osal/src/osal_heap.c
    osal/src/osal_log.c
    osal/src/osal_hooks.c
    osal/src/osal_utils.c

    ${FRTOS}/list.c
    ${FRTOS}/queue.c
    ${FRTOS}/tasks.c
    ${FRTOS}/timers.c
    ${FRTOS}/event_groups.c
    ${FRTOS}/stream_buffer.c
    ${FRTOS}/portable/MemMang/heap_4.c
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c

    # ST HAL pieces our hal/ uses (NS-accessible peripherals only)
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_cortex.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gpio.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc_ex.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr_ex.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart.c
    ${CUBE}/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart_ex.c
)

target_include_directories(tfm_ns PRIVATE
    app
    hal/include
    osal/include
    config
    Core/Inc
    ${FRTOS}/include
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure
    ${CUBE}/STM32L5xx_HAL_Driver/Inc
    ${CUBE}/CMSIS/Include
    ${CUBE}/CMSIS/Device/ST/STM32L5xx/Include
    ${CONFIG_SPE_PATH}/interface/include
)

target_compile_definitions(tfm_ns PRIVATE
    USE_HAL_DRIVER
    STM32L552xx
    DOMAIN_NS=1
    TFM_NS=1
)

target_link_libraries(tfm_ns PRIVATE
    tfm_api_ns
    tfm_api_ns_tz
)

# NS linker script (preprocessed template) + signing come from the SPE export.
# TODO(ns/README.md): wire target_add_scatter_file() / the tfm_ns_signed_bin
# custom target from ${CONFIG_SPE_PATH}/CMakeLists.txt.
