Notes when adding support for new MCU:

## Linker file

* Set FLASH origin to CORE_FLASH_START_ADDR
* Add fwMetadata section in text
* Add .noinit section

## Startup file:

* Replace SystemInit call with CoreSTM32F4SystemInit