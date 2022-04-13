#!/usr/bin/env bash

yaml_file=$1
gen_dir=$2
external_freq=$3
yaml_parser="dasel -n -p yaml --plain -f"
out_header="$gen_dir"/MCU.h
out_makefile="$gen_dir"/MCU.mk

echo "Generating MCU definitions..."

mkdir -p "$gen_dir"
echo "" > "$out_header"
echo "" > "$out_makefile"

mcu=$(basename "$yaml_file" .yml)
arch=$($yaml_parser "$yaml_file" arch)
mcu_family=$($yaml_parser "$yaml_file" mcuFamily)
vendor=$($yaml_parser "$yaml_file" vendor)
cpu=$($yaml_parser "$yaml_file" cpu)
freq=$($yaml_parser "$yaml_file" freq)
fpu=$($yaml_parser "$yaml_file" fpu)
float_abi=$($yaml_parser "$yaml_file" float-abi)
define_symbol=$($yaml_parser "$yaml_file" define-symbol)
app_metadata_offset=$($yaml_parser "$yaml_file" flash.app-metadata-offset)
uid_bits=$($yaml_parser "$yaml_file" uid-bits)
adc_bits=$($yaml_parser "$yaml_file" adc-bits)

{
    printf "%s\n\n" "#pragma once"
    printf "%s\n" "#include \"core/src/arch/common/Flash.h\""
} >> "$out_header"

{
    printf "%s\n" "ARCH := $arch"
    printf "%s\n" "MCU_FAMILY := $mcu_family"
    printf "%s\n" "VENDOR := $vendor"
    printf "%s\n" "MCU := $mcu"
    printf "%s\n" "CPU := $cpu"
    printf "%s\n" "FPU := $fpu"
    printf "%s\n" "FLOAT-ABI := $float_abi"
    printf "%s\n" "DEFINES += $define_symbol"
    printf "%s%x\n" "FW_METADATA_OFFSET := 0x" "$app_metadata_offset"
    printf "%s\n" "DEFINES += CORE_MCU_GENERATED"
} >> "$out_makefile"

if [[ "$external_freq" != "" ]]
then
    printf "%s\n" "DEFINES += CORE_EXT_CLOCK_FREQ_MHZ=$external_freq" >> "$out_makefile"
fi

if [[ $freq != "null" ]]
then
    printf "%s\n" "DEFINES += CORE_CPU_FREQ_MHZ=$freq" >> "$out_makefile"
else
    # If the MCU doesn't specify frequency in yaml file, use external clock frequency as core frequency
    printf "%s\n" "DEFINES += CORE_CPU_FREQ_MHZ=$external_freq" >> "$out_makefile"
fi

if [[ $($yaml_parser "$yaml_file" flash) != "null" ]]
then
    declare -i number_of_flash_pages

    if [[ $($yaml_parser "$yaml_file" flash.pages) != "null" ]]
    then
        number_of_flash_pages=$($yaml_parser "$yaml_file" flash.pages --length)

        printf "%s\n\n" "#define TOTAL_FLASH_PAGES $number_of_flash_pages" >> "$out_header"

        for ((i=0; i<number_of_flash_pages; i++))
        do
            addressStart=$($yaml_parser "$yaml_file" flash.pages.["$i"].address)
            page_size=$($yaml_parser "$yaml_file" flash.pages.["$i"].size)

            {
                printf "%s\n" "#ifndef FLASH_PAGE_ADDRESS_${i}"
                printf "%s\n" "#define FLASH_PAGE_ADDRESS_${i} $addressStart"
                printf "%s\n" "#endif"
                printf "%s\n" "#ifndef FLASH_PAGE_SIZE_${i}"
                printf "%s\n" "#define FLASH_PAGE_SIZE_${i} $page_size"
                printf "%s\n" "#endif"
            } >> "$out_header"
        done

        {
            printf "%s\n" "namespace {"
            printf "%s\n" "constexpr inline core::mcu::flash::flashPage_t PAGE_DESCRIPTOR[TOTAL_FLASH_PAGES] = {"
        } >> "$out_header"

        for ((i=0; i<number_of_flash_pages; i++))
        do
            {
                printf "%s\n" "{"
                printf "%s\n" ".address = FLASH_PAGE_ADDRESS_${i}",
                printf "%s\n" ".size = FLASH_PAGE_SIZE_${i}",
                printf "%s\n" "},"
            } >> "$out_header"
        done

        {
            printf "%s\n" "};"
            printf "%s\n" "}"
        } >> "$out_header"

        {
            printf "%s\n" "#define _FLASH_PAGE_ADDRESS_GEN(x) FLASH_PAGE_ADDRESS_##x"
            printf "%s\n" "#define FLASH_PAGE_ADDRESS(x)      _FLASH_PAGE_ADDRESS_GEN(x)"

            printf "%s\n" "#define _FLASH_PAGE_SIZE_GEN(x) FLASH_PAGE_SIZE_##x"
            printf "%s\n" "#define FLASH_PAGE_SIZE(x)      _FLASH_PAGE_SIZE_GEN(x)"
        } >> "$out_header"
    else
        # all flash pages have common size
        page_size=$($yaml_parser "$yaml_file" flash.page-size)
        flash_size=$($yaml_parser "$yaml_file" flash.size)
        number_of_flash_pages=$((flash_size/page_size))

        {
            printf "%s\n" "#define TOTAL_FLASH_PAGES $number_of_flash_pages"
            printf "%s\n" "#define FLASH_PAGE_SIZE_COMMON $page_size"
            printf "%s\n" "#define FLASH_END $((flash_size-1))"
        } >> "$out_header"

        addressStart=$($yaml_parser "$yaml_file" flash.flash-start)

        {
            printf "%s\n" "constexpr uint32_t FLASH_PAGE_SIZE(size_t page) {"
            printf "%s\n" "return FLASH_PAGE_SIZE_COMMON; }"

            printf "%s\n" "constexpr uint32_t FLASH_PAGE_ADDRESS(size_t page) {"
            printf "%s\n" "return FLASH_PAGE_SIZE_COMMON * page; }"
        } >> "$out_header"
    fi
fi

if [[ $($yaml_parser "$yaml_file" eeprom) != "null" ]]
then
    eeprom_size=$($yaml_parser "$yaml_file" eeprom.size)
    printf "%s\n" "#define EEPROM_END $((eeprom_size-1))" >> "$out_header"
fi

number_of_uart_interfaces=$($yaml_parser "$yaml_file" peripherals.uart)
number_of_i2c_interfaces=$($yaml_parser "$yaml_file" peripherals.i2c)

if [[ $($yaml_parser "$yaml_file" peripherals.custom-pins) == "true" ]]
then
    printf "%s\n" "DEFINES += CUSTOM_PERIPHERAL_PINS" >> "$out_makefile"
fi

{
    printf "%s\n" "namespace core::mcu::peripherals {"
    printf "%s\n" "constexpr inline size_t MAX_UART_INTERFACES = ${number_of_uart_interfaces};"
    printf "%s\n" "constexpr inline size_t MAX_I2C_INTERFACES = ${number_of_i2c_interfaces};"
    printf "%s\n" "}"

    printf "%s\n" "namespace core::mcu::adc {"
    printf "%s\n" "constexpr inline uint32_t MAX = $((2 ** adc_bits - 1));"
    printf "%s\n" "}"

    printf "%s\n" "namespace core::mcu {"
    printf "%s\n" "constexpr inline size_t UID_BITS = ${uid_bits};"
    printf "%s\n" "}"
} >> "$out_header"