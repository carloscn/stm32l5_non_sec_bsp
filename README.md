# Test STM32L55

## Toolchains:
* https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
* Note1 : recommand the 13.3.Rel1
* Note2 : linux system, can use `curl -sk https://raw.githubusercontent.com/carloscn/script/master/down_tool_chains/down_toolchain_13.2-rel1.sh | bash`

## Compile code

1. Config your cross compile tool path in `prj.cfg`
2. `source prj.cfg`
3. `git submodule update --init --recursive`
4. `make -j8`

## Donwload code

There are `st_flash` and `stm32cube_programmer` for your selection:
1. Link your stm32 into the host's usb.
2. `bash install_st_flash.sh` or download https://www.st.com/en/development-tools/stm32cubeprog.html
3. `bash flash.sh` (by config `USE_ST_FLASH` in `flash.sh`)

Note,
* stm32cubeprog can be supported by MAC-OS/WIN/Linux
* st-flash can be supported by WIN and Linux.

The program auto runs.

# About AAL and HAL

based on the ST company's demo baord, [NUCLEO-L-552ZE-Q](https://www.st.com/en/evaluation-tools/nucleo-l552ze-q.html)

Creating an Architecture Abstraction Layer (AAL) is a step above the HAL (Hardware Abstraction Layer) and is meant to abstract the underlying processor or microcontroller architecture details. This is particularly useful when you want to make your software portable across different processors or architectures while hiding low-level details like cache management, CPU modes, and system control registers.

When developing the Hardware Abstraction Layer (HAL) for an embedded hardware board, there are several common HAL modules you typically need to implement, depending on the board's resources and functional requirements.

For a chip's peripherals, such as cache management, interrupts, or memory protection, the architecture abstraction layer needs to provide a clean and uniform interface across various architectures.

Here are some common areas to consider when building an Architecture Abstraction Layer (AAL), particularly for processor-specific features like cache, memory management, and low-level control:

## Common AAL Modules

1. Cache Management : Most modern architectures like ARM, RISC-V, and x86 have their own mechanisms for managing the CPU cache (instruction cache, data cache).
2. Interrupt Handling
3. Memory Management and Protection
4. System Control and Low-Level Features
5. CPU Modes and Privileged Operations
6. Synchronization and Multicore Support
7. Debugging and Performance Monitoring

## Common HAL Modules

* HAL_GPIO (General Purpose Input/Output) : Provides control over GPIO pins for input and output operations. This is one of the most fundamental HAL modules, used to interact with external devices or control peripherals on the board.
* HAL_UART (Universal Asynchronous Receiver-Transmitter): Provides interfaces for serial communication, often used for debugging or communication between devices.
* HAL_SPI (Serial Peripheral Interface): Enables communication over the SPI bus, typically used for high-speed data exchange with external devices like sensors or memory.
* HAL_I2C (Inter-Integrated Circuit): Facilitates I2C communication, commonly used for interfacing with low-speed peripherals such as EEPROMs and sensors.
* HAL_FLASH Manages read and write operations to onboard flash memory, which is often required for firmware updates or storing configuration data.
* HAL_ADC (Analog-to-Digital Converter): Handles the conversion of analog signals to digital, frequently used for reading sensor values or measuring voltage levels.
* HAL_DAC (Digital-to-Analog Converter): Converts digital signals to analog outputs, typically used for generating analog voltages or audio signals.
* HAL_PWM (Pulse Width Modulation): Provides PWM signal generation, which is useful for controlling devices like motors, LEDs, or any component that needs precise signal timing.
* HAL_TIMER: Manages system timing, delays, and event triggers through timers. This module is essential for task scheduling and time-based events.
* HAL_RTC (Real-Time Clock): Provides real-time clock functionality for managing system time and date. This is often required for timestamping events or timed wake-up tasks.
* HAL_CAN (Controller Area Network): If the hardware supports a CAN bus, this module is needed for communication over the CAN bus, which is commonly used in automotive and industrial applications.
* HAL_WDG (Watchdog Timer) Implements a watchdog timer to monitor system health and prevent the system from locking up due to software errors.
* HAL_USB (Universal Serial Bus) Provides an interface for USB communication, which might be used for debugging, data transfer, or device management.
* HAL_SDIO (Secure Digital Input Output) Manages read and write operations to SD cards, typically used if the hardware has an SD card slot or similar external storage devices.
* HAL_ETH (Ethernet) Implements Ethernet functionality for network communication, often required for systems that connect to a LAN or need internet access.
* HAL_SPI_FLASH (External SPI Flash Memory) If the board includes external SPI Flash memory, you need a module for reading from and writing to this external storage.

## Other Potential HAL Modules
Depending on the specific hardware capabilities, you may need additional HAL modules, such as:

* HAL_LCD: If the board has an LCD display interface.
* HAL_TOUCH: For handling touch input if the device has a touch screen.
* HAL_AUDIO: For processing audio data if audio functionality is supported.