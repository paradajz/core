Notes when adding support for new MCU:

## Linker file

* Set conditional origin for FLASH
* Add fwMetadata section in text
* Add .noinit section

## Startup file:

* Replace SystemInit call with CoreSTM32F4SystemInit