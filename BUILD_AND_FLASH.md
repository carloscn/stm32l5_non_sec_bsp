# 构建与烧录 — STM32L552 / NUCLEO-L552ZE-Q

三个可交付物,两条构建线:

| 目标 | 工程 | 构建方式 | 产物 | 运行前提 |
|---|---|---|---|---|
| **独立镜像**(裸机 + FreeRTOS,无 TF-M) | `stm32l5_non_sec_bsp` | `make` | `build/stm32l5_non_secure_bsp.{elf,hex,bin}` | `TZEN=0` |
| **TF-M SPE**(BL2 + tfm_s) | `stm32l5_secure_bsp` | `./build.sh`(CMake/Ninja) | `build/bin/{bl2,tfm_s,tfm_s_signed}.bin` | `TZEN=1` + provision |
| **TF-M NSPE**(FreeRTOS + PSA 冒烟) | `stm32l5_non_sec_bsp` | 根 `CMakeLists.txt` | `build_ns/bin/tfm_ns_signed.bin` | 配 SPE 一起烧 |

---

## 0. 环境

| 组件 | 位置 / 版本 |
|---|---|
| 交叉工具链 | `C:\opt\cross_compile\arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi` |
| Shell | **Git Bash**(不是 WSL) |
| CMake / Ninja | Strawberry 自带,`C:\opt\Strawberry\c\bin` |
| Python venv(imgtool / cbor2 签名用) | `stm32l5_secure_bsp\.venv`(`./setup.sh` 建) |
| STM32CubeProgrammer | ≥ 2.23,`C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer` |
| **ST-LINK 固件** | **≥ V2J48**。旧版(V2J47)会导致 option byte / TZEN 操作失败。升级:`"...\bin\jre\bin\javaw.exe" -jar "...\Drivers\FirmwareUpgrade\STLinkUpgrade.jar"` → Open in update mode → Upgrade |

每条命令前先在工程根 `source prj.cfg`(设 `GCC_PATH` / 把 CubeProgrammer 加进 PATH)。

---

## 1. 独立镜像(`stm32l5_non_sec_bsp`,Makefile)

```bash
cd stm32l5_non_sec_bsp
git submodule update --init --recursive     # FreeRTOS + STM32CubeL5
source prj.cfg
make -j
```
产物:`build/stm32l5_non_secure_bsp.hex`(链在 `0x08000000`,`ARM_CM33_NTZ` 端口,110 MHz)。

> 该镜像按 **TZEN=0** 设计。当前板子是 TZEN=1(见 §4),**SWD 无法把 TZEN 关掉**(`-tzenreg` 只支持 USB DFU bootloader,板载 ST-LINK/V2-1 不支持;`mode=POWERDOWN` 也不支持)。要跑独立镜像需 USB-C 接 USB_USER 口 + BOOT0 拉高 + `port=USB1 -tzenreg`。

烧(TZEN=0 后):
```bash
STM32_Programmer_CLI -c port=SWD mode=UR -d build/stm32l5_non_secure_bsp.hex -rst
```

---

## 2. TF-M SPE(`stm32l5_secure_bsp`)

```bash
cd stm32l5_secure_bsp
./setup.sh                                   # 一次:拉 trusted-firmware-m (TF-Mv2.1.1) + 建 .venv
source prj.cfg
source .venv/Scripts/activate
./build.sh
```
`config/stm32l5_secure_bsp.cmake` 关键项:
- `TFM_PLATFORM = stm/nucleo_l552ze_q`
- `TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH = config/crypto_config_l5_smoketest.h` —— profile_medium **+ `PSA_WANT_ALG_CMAC`**(ECDSA P-256 profile 里本来就有;Ed25519 mbed-crypto 不支持)
- `TFM_SPM_LOG_LEVEL / TFM_PARTITION_LOG_LEVEL = INFO`、`MCUBOOT_LOG_LEVEL = INFO` —— 否则串口全静默

产物:`build/bin/{bl2.bin, bl2.hex, tfm_s.bin, tfm_s_signed.bin}`
NS 接口导出:`build/api_ns/`(给 §3 用)

改 `config/` 后重编要清 cache:
```bash
rm -f build/build.ninja build/CMakeCache.txt build.log && ./build.sh
```

---

## 3. TF-M NSPE(`stm32l5_non_sec_bsp`,CMake)

```bash
cd stm32l5_non_sec_bsp
source prj.cfg
export PATH="$(cygpath -u "$GCC_PATH"):/c/opt/Strawberry/c/bin:$PATH"

SPE="$(cygpath -m "$PWD/../stm32l5_secure_bsp/build/api_ns")"
VENV_PY="$(cygpath -m ../stm32l5_secure_bsp/.venv/Scripts/python.exe)"

rm -rf build_ns
cmake -S . -B build_ns -G Ninja \
      -DCONFIG_SPE_PATH="$SPE" \
      -DCMAKE_TOOLCHAIN_FILE="$SPE/cmake/toolchain_ns_GNUARM.cmake" \
      -DCMAKE_BUILD_TYPE=MinSizeRel \
      -DPython3_EXECUTABLE="$VENV_PY"
cmake --build build_ns
```
产物:`build_ns/bin/{tfm_ns.elf, tfm_ns.bin, tfm_ns_signed.bin}`

要点(都已写进 `CMakeLists.txt` / 源码):
- `-DCMAKE_BUILD_TYPE=MinSizeRel` —— NS 分区只有 **27 KiB 可用**(`FLASH_NS_PARTITION_SIZE=0x9000` − header − trailer),`-O0` 会溢出
- `-DPython3_EXECUTABLE=<venv python>` —— 签名脚本要 `cbor2` / `imgtool`,系统 Python 没有
- ABI 必须跟 SPE 一致:`cortex-m33` + **soft-float**(CMakeLists 在 `project()` 前 `include` 了 `cpuarch.cmake` / `spe_config.cmake`)
- `platform_ns` 已提供 ST HAL 核心 + NS startup + `system_stm32l5xx.c`,不要重复加;不要加 `-ICore/Inc`(那份 `hal_conf` 会拉 ADC/RTC 头)

---

## 4. 板子 option bytes 现状

```
RDP         : 0xAA  (Level 0)          ← 可调,别动到 0xCC
TZEN        : 0x1   (TrustZone 开)
SECBOOTADD0 : 0x0C000000               ← 从安全 BL2 启动
SECWM1      : 全 bank1 安全
SECWM2      : 首次 SPE 启动后被 dummy provisioning 改成「page 0-1 安全,其余非安全」
```
这组值是 TF-M `build/api_ns/regression.sh` 写的。要恢复干净非 TrustZone 状态:`-ob TZEN=0`(需 USB DFU,见 §1)。

新板子/被擦过要重新 provision(在 `stm32l5_secure_bsp/build/api_ns/` 下):
```bash
bash regression.sh          # RDP=0xAA TZEN=1 + 水印 + SECBOOTADD0（不烧镜像）
```

---

## 5. 烧录(TF-M 全链路)

地址(本布局,已验证):

| 镜像 | 地址 | 说明 |
|---|---|---|
| `bl2.hex` | 内置 `0x0C000000` | 直接 `-d bl2.hex` |
| `tfm_s_signed.bin` | `0x0C014000` | **安全**别名(S 槽 = `FLASH_AREA_0_OFFSET`) |
| `tfm_ns_signed.bin` | `0x08041000` | **非安全**别名!SPE provisioning 后 bank2 那段是非安全,写 `0x0C041000` 会 verify 失败 |

```bash
export PATH="/c/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin:$PATH"
cd stm32l5_secure_bsp/build/bin
STM32_Programmer_CLI -c port=SWD mode=UR -d bl2.hex -v
STM32_Programmer_CLI -c port=SWD mode=UR -d tfm_s_signed.bin 0x0C014000 -v

cd ../../../stm32l5_non_sec_bsp/build_ns/bin
STM32_Programmer_CLI -c port=SWD mode=UR -d tfm_ns_signed.bin 0x08041000 -v

STM32_Programmer_CLI -c port=SWD mode=UR -rst
```

只更新某一块就只烧那一块(BL2 / SPE 不变时只重烧 `tfm_ns_signed.bin`)。

---

## 6. 看串口

`COM4`(ST-LINK VCP)@ **115200 8N1**。Git Bash 里用 PowerShell 抓:

```bash
powershell.exe -NoProfile -Command "\$p=New-Object System.IO.Ports.SerialPort('COM4',115200,'None',8,'One'); \$p.ReadTimeout=400; \$p.Open(); \$b=''; \$sw=[Diagnostics.Stopwatch]::StartNew(); while(\$sw.Elapsed.TotalSeconds -lt 20){ try{ \$b+=\$p.ReadExisting() }catch{}; Start-Sleep -Milliseconds 40 }; \$p.Close(); [IO.File]::WriteAllText('C:\Users\CARLOS~1\AppData\Local\Temp\com.txt',\$b)" &
sleep 1
STM32_Programmer_CLI -c port=SWD mode=UR -rst
wait
sed 's/\x1b\[[0-9;]*m//g' /c/Users/CARLOS~1/AppData/Local/Temp/com.txt
```

期望输出:
```
[INF] Starting bootloader
[INF] Image index: 1 ... Image index: 0 ... Jumping to the first image slot     ← BL2 校验 S+NS
Booting TF-M v2.1.1-LTS
[Sec Thread] Secure image initializing!
[INF][Crypto] Init HW accelerator... complete.
[0000] stm32l5_non_sec_bsp (NSPE): FreeRTOS V11.1.0 up
[0001]   CMAC rfc4493-ex2: ok
[0002]   CMAC rfc4493-ex4: ok
[0003] CMAC  PASS                          ← RFC 4493,走 psa_mac_compute → TF-M Crypto 分区
[0004]   ECDSA KAT: verify(good) ok
[0005]   ECDSA KAT: verify(bad) rejected ok
[0006]   ECDSA rt: gen/sign/verify ok
[0007] ECDSA P-256  PASS                   ← RFC 6979 KAT + 生成/签/验往返
```

---

## 7. 已知问题

| 问题 | 现象 | 备注 |
|---|---|---|
| **NSPE 调度器复位循环** | `[0007]` 之后 `osal_sched_start()` → 板子复位,从头再来 | crypto 冒烟(`main()` 里,调度器之前)完整跑完并 PASS。halt 时 PC 在 secure `p256-m`(即复位后重跑测试),debugger 读 NS `SCB->VTOR` = 0。FreeRTOS `ARM_CM33_NTZ` 端口在 TF-M NS 下启动首任务还需调。独立镜像不受影响。 |
| **TZEN 关不掉** | `-ob TZEN=0` / `-tzenreg` 在 SWD 下失败 | 需 USB DFU(BOOT0 高 + USB_USER 口 + `port=USB1 -tzenreg`) |
| **LD3(红,PG2)可能不亮** | 需要 VDDIO2,NS 侧可能无权开 PWR | LD1(PC7)/ LD2(PB7)正常 |

---

## 8. 调试(可选,OpenOCD + gdb)

```bash
openocd -f interface/stlink.cfg -f target/stm32l5x.cfg
# 另一个终端
arm-none-eabi-gdb build_ns/bin/tfm_ns.elf -ex "target extended-remote :3333"
```
或装 **STM32CubeCLT**(打包了 CubeProgrammer CLI + ST-LINK GDB server + OpenOCD)。
