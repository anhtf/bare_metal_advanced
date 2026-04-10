# STM32 Bare Metal Development

A comprehensive bare metal firmware development project for STM32F4 microcontroller series, focusing on learning and implementing low-level hardware drivers and embedded systems concepts.

## Overview

This project provides a foundation for developing bare metal firmware on STM32F4 microcontrollers without relying on HAL (Hardware Abstraction Layer) libraries. It serves as a practical learning platform for understanding embedded systems development, register-level programming, and STM32 architecture.

## Target Hardware

- **MCU**: STM32F407VET6
- **Core**: ARM Cortex-M4
- **Architecture**: 32-bit ARM
- **FPU**: Single-precision Floating Point Unit
- **DSP**: Available
- **MPU**: Memory Protection Unit
- **Endianness**: Little-endian

## Project Structure

```
STM32BareMetal/
├── Src/                      # Source files
│   ├── main.c               # Main application entry point
│   ├── startup_stm32f407xx.S # Startup assembly code
│   ├── syscall.c            # System call implementations
│   └── sysmem.c             # Memory management
├── mcu_headers/             # MCU-specific headers
│   └── CMSIS/               # CMSIS (Cortex Microcontroller Software Interface Standard)
├── cmake/                   # CMake configuration files
│   ├── gnu-tools-for-stm32.cmake
│   └── vscode_generated.cmake
├── CMakeLists.txt           # CMake build configuration
├── CMakePresets.json        # CMake presets for different build types
└── stm32f407xe_flash.ld     # Linker script for Flash memory layout
```

## Prerequisites

### Required Tools

- **ARM GCC Toolchain**: GNU Arm Embedded Toolchain
  - Download: https://developer.arm.com/Tools-and-Software/GNU-Toolchain
  - Includes: `arm-none-eabi-gcc`, `arm-none-eabi-g++`, `arm-none-eabi-gdb`

- **CMake**: Version 3.20 or higher
  ```bash
  sudo apt-get install cmake
  ```

- **Ninja Build System**:
  ```bash
  sudo apt-get install ninja-build
  ```

- **Visual Studio Code**:
  - STM32 Visual Studio Code Extension
  - C/C++ IntelliSense Extension
  - CMake Tools Extension

### Optional Tools

- **OpenOCD**: For debugging and flash programming
- **STLink Tools**: For STLink debugger support

## Building the Project

### Using CMake Presets

The project includes CMake presets for different build configurations:

```bash
# Configure the project in Debug mode
cmake --preset Debug

# Build the project
cmake --build --preset Debug

# Build Release version
cmake --preset Release
cmake --build --preset Release
```

### Build Output

The compiled artifacts will be placed in:
```
build/Debug/    # Debug build
build/Release/  # Release build
```

Generated files include:
- `STM32BareMetal.elf` - Executable with debugging symbols
- `STM32BareMetal.bin` - Binary file for flashing
- `STM32BareMetal.hex` - Intel HEX format for programming

## Available Lab Branches

This repository includes dedicated branches for different learning modules:

1. **Lab 01 - Getting Started** (`Cource/Lab_01_Getting_Started`)
   - Basic project setup
   - LED blinking implementation
   - GPIO configuration

2. **Lab 02 - Essentials of Firmware Debugging** (`Cource/Lab_02_Essentials_of_Firmware_Debugging`)
   - Debugging techniques
   - SWD (Serial Wire Debug) configuration
   - Breakpoints and watch expressions

3. **Lab 03 - Memory-to-Memory DMA** (`Cource/Lab_03_DMA`)
   - Direct Memory Access (DMA) controllers
   - DMA transfers and configuration

4. **Lab 04 - ADC DMA Drivers**
   - ADC (Analog-to-Digital Converter) integration
   - DMA-based data acquisition

5. **Lab 05 - ADC Timer Trigger DMA Drivers**
   - Timer-triggered ADC conversions
   - Advanced DMA sequencing

6. **Lab 06 - UART DMA Drivers**
   - Serial communication via UART
   - DMA-based UART data handling

7. **Lab 07 - SPI DMA Drivers**
   - SPI (Serial Peripheral Interface) protocol
   - DMA-accelerated SPI transfers

8. **Lab 08 - I2C DMA Drivers**
   - I2C (Inter-Integrated Circuit) protocol
   - DMA-based I2C operations

9. **Lab 09 - PWM DMA Drivers**
   - PWM (Pulse Width Modulation) generation
   - DMA-controlled PWM patterns

10. **Lab 10 - Advanced Debugging Techniques**
    - In-circuit debugging
    - Real-time monitoring
    - Advanced GDB usage

11. **Lab 11 - RTC Drivers**
    - Real-Time Clock (RTC) configuration
    - Calendar and alarm functionality

12. **Lab 12 - Independent Watchdog (IWDG) Timer Drivers**
    - Watchdog timer configuration
    - Reset functionality

13. **Lab 13 - Windowed Watchdog (WWDG) Timer Drivers**
    - Windowed watchdog operation
    - Safety mechanisms

14. **Lab 14 - Standby Mode and Wakeup Drivers**
    - Low-power standby modes
    - Wake-up mechanisms

### Switching to a Lab Branch

```bash
# View all available branches
git branch -a

# Switch to a specific lab
git checkout Cource/Lab_01_Getting_Started

# Create a new branch from a lab
git checkout -b my-feature Cource/Lab_03_DMA
```

## Development Workflow

### 1. Clone and Setup

```bash
git clone <repository-url>
cd STM32BareMetal
```

### 2. Configure Build Environment

```bash
cmake --preset Debug
```

### 3. Build Project

```bash
cmake --build --preset Debug
```

### 4. Flash to Device

Using STLink (via STM32CubeIDE or OpenOCD):
```bash
openocd -f board/stm32f4discovery.cfg \
  -c "program build/Debug/STM32BareMetal.bin verify reset exit 0x08000000"
```

### 5. Debug with GDB

```bash
arm-none-eabi-gdb build/Debug/STM32BareMetal.elf
```

## Key Features

- **Bare Metal Development**: Direct register manipulation without HAL abstraction
- **CMSIS Integration**: Using ARM CMSIS standards for portability
- **Modern CMake Build System**: Clean, scalable build configuration
- **Multiple Peripheral Drivers**: GPIO, UART, SPI, I2C, ADC, DMA, PWM, RTC, Watchdog
- **Comprehensive Learning Path**: Progressive lab structure from basics to advanced topics
- **VS Code Integration**: Full IDE support with debugging and IntelliSense

## Configuration

### Build Variants

The project supports two main build types:

- **Debug**: Includes debugging symbols, optimizations disabled (`-g -O0`)
- **Release**: Optimized code, minimal symbols (`-O3`)

### MCU Configuration

Edit `CMakePresets.json` to modify MCU parameters:
- `CMSIS_Dcore`: CPU core type
- `CMSIS_Dfpu`: Floating point unit configuration
- `CMSIS_Ddsp`: DSP capability
- `CMSIS_Dmpu`: Memory Protection Unit

## Memory Layout

The linker script `stm32f407xe_flash.ld` defines:
- **Flash Memory**: Code and initialized data storage
- **RAM**: Runtime data and stack
- **Stack**: Top of RAM
- **Heap**: Between BSS and stack

Modify the linker script for custom memory layouts or multiple memory regions.

## Troubleshooting

### Build Issues

- **CMake not found**: Install CMake 3.20 or higher
- **ARM toolchain not in PATH**: Add toolchain bin directory to system PATH
- **Ninja not found**: Install ninja-build package

### Debugging Issues

- **GDB connection fails**: Verify OpenOCD is running and STLink is connected
- **Flash programming fails**: Check STLink driver installation
- **Code execution halts**: Verify linker script memory mapping

## References

- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM CMSIS Documentation](https://open-cmsis-pack.github.io/Open-CMSIS-Pack-Spec/)
- [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools-and-Software/GNU-Toolchain)
- [CMake Documentation](https://cmake.org/documentation/)

## License

This project is provided under the STMicroelectronics license. See LICENSE file for details.

## Contributing

1. Create a feature branch from the appropriate lab branch
2. Implement your changes
3. Test thoroughly on hardware
4. Submit pull requests with clear descriptions

## Support

For issues and questions:
- Check existing branches for similar implementations
- Refer to STM32 Reference Manual for hardware details
- Review CMSIS documentation for standardization

---

**Last Updated**: April 2026
