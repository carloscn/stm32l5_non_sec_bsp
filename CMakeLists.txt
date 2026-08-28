#-------------------------------------------------------------------------------
#  stm32l5_non_sec_bsp -- TF-M Non-Secure app (NSPE)
#
#  Consumes the SPE export from ../stm32l5_secure_bsp/build/api_ns and produces
#  tfm_ns.bin / tfm_ns_signed.bin. Runs the PSA crypto smoke tests (app/) via
#  the TF-M Crypto partition.
#
#  Build (from the repo root, in Git Bash, after `source prj.cfg`):
#     SPE=$(cygpath -m "$PWD/../stm32l5_secure_bsp/build/api_ns")
#     cmake -S . -B build_ns -G Ninja \
#           -DCONFIG_SPE_PATH="$SPE" \
#           -DTFM_TOOLCHAIN_FILE="$SPE/cmake/toolchain_ns_GNUARM.cmake"
#     cmake --build build_ns
#
#  The standalone (no-TF-M) image is still built with the Makefile.
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR
        "CONFIG_SPE_PATH ('${CONFIG_SPE_PATH}') missing. Point it at "
        "../stm32l5_secure_bsp/build/api_ns")
endif()

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

# These must be visible BEFORE project() so the NS toolchain file picks up the
# right -mcpu / -march / float ABI (must match the SPE: cortex-m33, soft float).
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)
include(${CONFIG_SPE_PATH}/cmake/spe_config.cmake)

if (NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CONFIG_SPE_PATH}/cmake/toolchain_ns_GNUARM.cmake)
endif()

project("stm32l5_non_sec_bsp_ns" VERSION 0.1 LANGUAGES C ASM)

# The spe subdir (api_ns/CMakeLists.txt) includes spe_export itself.

# tfm_api_ns, tfm_api_ns_tz, platform_ns, platform_region_defs, NS linker script,
# and the tfm_ns_signed_bin custom target.
add_subdirectory(${CONFIG_SPE_PATH} spe)

set(REPO  ${CMAKE_CURRENT_SOURCE_DIR})
set(FRTOS ${REPO}/FreeRTOS)

add_executable(tfm_ns)
set_target_properties(tfm_ns PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_sources(tfm_ns PRIVATE
    ${REPO}/Core/Src/main.c
    ${REPO}/Core/Src/hal_tick_dwt.c
    ${REPO}/Core/Src/syscalls_ns.c
    ${REPO}/Core/Src/fault_ns.c
    # NS startup (from platform_ns) provides the fault handlers; no stm32l5xx_it.c
    # and no Core/Inc (that hal_conf pulls ADC/RTC headers platform_ns lacks).

    ${REPO}/app/led.c
    ${REPO}/app/heartbeat.c
    ${REPO}/app/test_psa_cmac.c
    ${REPO}/app/test_psa_ecdsa_p256.c
    ${REPO}/app/ns/os_wrapper_osal.c

    ${REPO}/hal/src/hal_error.c
    ${REPO}/hal/src/hal_gpio.c
    ${REPO}/hal/src/hal_uart.c
    ${REPO}/hal/src/hal_irq.c
    ${REPO}/hal/src/hal_mcu.c
    ${REPO}/hal/src/hal_cache.c

    ${REPO}/osal/src/osal_error.c
    ${REPO}/osal/src/osal_task.c
    ${REPO}/osal/src/osal_mutex.c
    ${REPO}/osal/src/osal_sem.c
    ${REPO}/osal/src/osal_queue.c
    ${REPO}/osal/src/osal_sched.c
    ${REPO}/osal/src/osal_heap.c
    ${REPO}/osal/src/osal_log.c
    ${REPO}/osal/src/osal_hooks.c
    ${REPO}/osal/src/osal_utils.c

    ${CONFIG_SPE_PATH}/interface/src/os_wrapper/tfm_ns_interface_rtos.c

    ${FRTOS}/list.c
    ${FRTOS}/queue.c
    ${FRTOS}/tasks.c
    ${FRTOS}/timers.c
    ${FRTOS}/event_groups.c
    ${FRTOS}/stream_buffer.c
    ${FRTOS}/portable/MemMang/heap_4.c
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
)

target_include_directories(tfm_ns PRIVATE
    ${REPO}/app
    ${REPO}/hal/include
    ${REPO}/osal/include
    ${REPO}/config
    ${FRTOS}/include
    ${FRTOS}/portable/GCC/ARM_CM33_NTZ/non_secure
)

target_compile_definitions(tfm_ns PRIVATE
    USE_HAL_DRIVER
    STM32L552xx
    TFM_NS=1
)

# tfm_api_ns brings platform_ns (ST HAL + NS startup + system_stm32l5xx.c) and
# the psa/ interface headers, transitively.
target_link_libraries(tfm_ns PRIVATE
    tfm_api_ns
    tfm_api_ns_tz
)

# NS linker script (appli_ns.ld, preprocessed) + .bin/.hex/.elf, from the
# toolchain-file macros.
target_add_scatter_file(tfm_ns ${CONFIG_SPE_PATH}/platform/linker_scripts)
add_convert_to_bin_target(tfm_ns)
