# AVR core modules

This repository contains several modules for easier control of integrated AVR peripherals:

- ADC
- Helpers (Bit and pin manipulation)
- Reset
- SPI
- Strings
- Timing
- UART

Modules are tested for ATmega32u4, AT90USB1286 and ATmega2560.

## ADC

For ADC, analog reference and prescaler must be defined as symbols. Also, `USE_ADC` must be defined to enabled ADC.

### Prescaler

Valid options are 128, 64, 32 and 16. To define prescaler, use one of the following symbols:
- `ADC_PRESCALER=128`
- `ADC_PRESCALER=64`
- `ADC_PRESCALER=32`
- `ADC_PRESCALER=16`

### Analog reference

Reference can be set to internal 1.1V, internal 2.56V, AVCC reference or AREF. To define reference use one of the following symbols:

- `VREF_AREF`
- `VREF_AVCC`
- `VREF_INTERNAL_2V56`
- `VREF_INTERNAL_1V1`

### Enabling ADC

In order to actually enable ADC, additional symbol must be defined.

- `USE_ADC`

## Strings

This module provides an easy way to manipulate C-style strings. To use strings, `USE_STRINGS` symbol must be defined and `STRING_BUFFER_SIZE` symbol must be defined with wanted value, for instance 50:

- `STRING_BUFFER_SIZE=50`

## UART

To use UART, `USE_RX` symbol must be defined to use UART receiver and `USE_TX` symbol must be defined to use UART transmitter.

- `USE_TX`
- `USE_RX`

Also, `UART_BUFFER_SIZE` symbol must be defined. For instance, to use buffer size of 30 bytes, use the following symbol:
- `UART_BUFFER_SIZE=30`