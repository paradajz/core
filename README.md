# Core modules

Collection of modules for easier control of integrated AVR peripherals, as well as some general helping macros and symbols.

This collection is split into two parts:

- AVR
- HAL

## General

This directory contains several modules which are independent of underlying architecture:

- Bit manipulation
    - Macros for easier bit access in variables and registers.
- Ring buffer
    - Contains templated ring buffer implementation.
- Timing
    - Includes simple delay function for AVR architecture, as well as `currentRunTimeMs()` function used to get current run time in milliseconds. This function uses `rTime_ms` variable which must be externally implemented and updated in order to use the function.
- StringBuilder
    - Helper class used for easier string manipulation.
- Misc
    - Includes various helper constants and macros.

## AVR

This directory contains some hardware abstraction APIs and macros for AVR architecture.

Contains following modules:

- ADC
- Pin manipulation
- Reset
- SPI
- I2C

Only ADC module requires configuration.

#### ADC

To configure ADC, structure of `conf_t` type must be intitialized. Structure holds two elements: `prescaler` and `vref`.
