# NS build (TF-M NSPE)  — status: WIP

Goal: build `stm32l5_non_sec_bsp` as the **Non-Secure app** running under the
SPE from `../stm32l5_secure_bsp`, so the PSA crypto smoke tests can exercise
**PSA crypto through TF-M** (AES-CMAC + ECDSA P-256 KATs) on the real board.

The standalone (no-TF-M) image is unchanged — still `make` in the repo root.

## Done

- `stm32l5_secure_bsp`: `PSA_WANT_ALG_CMAC` enabled in the PSA crypto config
  (`config/crypto_config_l5_smoketest.h`); `tfm_s` rebuilt, `psa_mac_compute` /
  `ecdsa` / `psa_verify_message` confirmed in `tfm_s.map`.
- `app/test_psa_cmac.c`      — RFC 4493 examples 2 & 4, via `psa_mac_compute`.
- `app/test_psa_ecdsa_p256.c` — RFC 6979 A.2.5 KAT verify + generate/sign/verify
  round trip + negative check, via `psa_verify_message` / `psa_sign_message`.
  (mbed-crypto has no Ed25519 backend — ECDSA P-256 is the PSA-supported
  signature primitive, same as the S32K312 reference `test_ecdsa.c`.)
- `Core/Src/main.c`          — single entry file for both builds. Under
  `#if defined(TFM_NS)` it adds `tfm_ns_interface_init()` +
  `test_psa_cmac()` + `test_psa_ecdsa_p256()`; `hal_mcu_init()` skips the clock tree when TFM_NS.
- `app/ns/os_wrapper_osal.c` — `os_wrapper/mutex.h` for
  `tfm_ns_interface_rtos.c`, implemented on `osal_mutex_*` (no direct
  FreeRTOS calls).
- `CMakeLists.txt`           — NS-app project skeleton (consumes `CONFIG_SPE_PATH`).

## Open (to finish the NS build)

1. **Linker script + signing.** Wire `${CONFIG_SPE_PATH}/platform/linker_scripts/appli_ns.ld`
   (needs C-preprocess with `region_defs.h`) onto `tfm_ns`, then reuse the
   `tfm_ns_signed_bin` custom command from `${CONFIG_SPE_PATH}/CMakeLists.txt`.
2. **NS console UART.** Confirm which UART the `stm/nucleo_l552ze_q` TF-M port
   leaves to NS (GTZC attribution). If LPUART1 is the SPE's secure log UART,
   NS access faults — `hal_uart.c` must target the NS-assigned instance
   (likely USART3 / or share LPUART1 if the port marks it NS).
3. **HAL_Init / timebase.** NS must not touch secure clock regs. Decide: keep
   `Core/Src/stm32l5xx_hal_timebase_tim.c` (TIM6 for HAL tick) but skip
   `SystemClock_Config`; ensure `HAL_Init()` on NS is safe (SysTick owned by
   FreeRTOS port; NS `startup_stm32l5xx_ns.c` sets VTOR).
4. **First `cmake --build build_ns`** — resolve the inevitable
   include/symbol clashes between our startup/vectors and
   `startup_stm32l5xx_ns.c` + `platform_ns`.
5. Sign with `../stm32l5_secure_bsp/scripts/sign_ns_image.sh` or the api_ns
   custom target; flash `bl2 + tfm_s + tfm_ns` via `flash.sh --regression`.

## Build (once wired)

```bash
SPE=$(realpath ../stm32l5_secure_bsp/build/api_ns)
cmake -S . -B build_ns -G Ninja \
      -DCONFIG_SPE_PATH="$SPE" \
      -DTFM_TOOLCHAIN_FILE="$SPE/cmake/toolchain_ns_GNUARM.cmake"
cmake --build build_ns
```
