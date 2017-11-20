# Core modules

Collection of modules for easier control of integrated AVR and STM32 peripherals, as well as some general helping macros and symbols.

This collection is split into two parts:

- General
- HAL

## General

This directory contains several modules which are independent of underlying architecture:

- Bit manipulation
    - Macros for easier bit access in variables and registers.
- Ring buffer
    - Contains lightweight ring buffer implementation from LUFA project.
- Strings
    - C++ class used for easier building of strings. `USE_STRINGS` symbol must be defined for compilation and `STRING_BUFFER_SIZE` symbol must be defined with wanted value, for instance 50: `STRING_BUFFER_SIZE=50`
- Timing
    - Includes simple delay function for AVR architecture, as well as `rTimeMs()` function used to get current run time in milliseconds. This function uses `rTime_ms` variable which must be externally implemented and updated in order to use the function.
- Misc
    - Includes various helper constants and macros.

## HAL

This directory contains some hardware abstraction APIs and macros for AVR architecture, as well as STM32 ARM microcontrollers which are using CubeMX HAL. For these microcontrollers, this module is just a wrapper around CubeMX HAL.

### AVR

Contains following modules:

- ADC
    - Used for easier access to ADC peripheral inside AVR.
- Pin manipulation
    - Used for easier pin access (read/write and setting input/output).
- Reset
    - Module used to initiate MCU reboot.
- SPI
    - Used for easier access to SPI peripheral inside AVR.
- Interrupt
    - Simple macros used to disable and enable all interrupts.

Only ADC module requires configuration.

#### ADC

##### Prescaler

Valid options are 128, 64, 32 and 16. To define prescaler, use one of the following symbols:
- `ADC_PRESCALER=128`
- `ADC_PRESCALER=64`
- `ADC_PRESCALER=32`
- `ADC_PRESCALER=16`

##### Analog reference

Reference can be set to internal 1.1V, internal 2.56V, AVCC reference or AREF. To define reference use one of the following symbols:

- `VREF_AREF`
- `VREF_AVCC`
- `VREF_INTERNAL_2V56`
- `VREF_INTERNAL_1V1`

##### Enabling ADC

In order to actually enable ADC, additional symbol must be defined.

- `USE_ADC`

### STM32

In order to use this module, CubeMX HAL must be used and `STM32` symbol must be defined.

- Pin manipulation
    - Used for easier pin access (read/write only).
- Interrupt
    - Simple macros used to disable and enable all interrupts.