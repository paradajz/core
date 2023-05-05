#!/usr/bin/env bash

for arg in "$@"; do
    case "$arg" in
        --yaml-file=*)
            yaml_file=${arg#--yaml-file=}
            ;;

        --gen-dir=*)
            gen_dir=${arg#--gen-dir=}
            ;;

        --external-freq=*)
            external_freq=${arg#--external-freq=}
            ;;

        --gen-usb=*)
            gen_usb=${arg#--gen-usb=}
            ;;
    esac
done

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

yaml_parser="dasel -n -p yaml --plain -f"
out_header="$gen_dir"/CoreMCUGenerated.h
out_makefile="$gen_dir"/CoreMCUGenerated.mk

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
app_metadata_offset=$($yaml_parser "$yaml_file" flash.app-metadata-offset)
uid_bits=$($yaml_parser "$yaml_file" uid-bits)
adc_bits=$($yaml_parser "$yaml_file" adc-bits)

{
    printf "%s\n\n" "#pragma once"
    printf "%s\n" "#include <inttypes.h>"
    printf "%s\n" "#include <stddef.h>"
    printf "%s\n" "#include \"core/arch/common/Flash.h\""
} >> "$out_header"

{
    printf "%s\n" "CORE_MCU_MODEL := $mcu"
    printf "%s\n" "CORE_MCU_ARCH := $arch"
    printf "%s\n" "CORE_MCU_VENDOR := $vendor"
    printf "%s\n" "CORE_MCU_FAMILY := $mcu_family"
    printf "%s%x\n" "CORE_MCU_FW_METADATA_OFFSET := 0x" "$app_metadata_offset"
    printf "%s\n" "CORE_MCU_LINKER_FILE := $script_dir/../src/core/arch/$arch/$vendor/variants/$mcu_family/$mcu/$mcu.ld"
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_GENERATED"
    printf "%s%s\n" "CORE_MCU_DEFINES += CORE_MCU_ARCH_" "${arch^^}"
    printf "%s%s\n" "CORE_MCU_DEFINES += CORE_MCU_VENDOR_" "${vendor^^}"
    printf "%s\n" 'CORE_MCU_LDFLAGS += -T $(CORE_MCU_LINKER_FILE)'
} >> "$out_makefile"

if [[ $arch == "avr" ]]
then
    {
        printf "%s\n" "CORE_MCU_LDFLAGS += -mmcu=$mcu"
        printf "%s\n" "CORE_MCU_CPPFLAGS += -mmcu=$mcu"
    } >> "$out_makefile"
fi

if [[ $cpu != "null" ]]
then
    {
        printf "%s\n" "CORE_MCU_CPU := $cpu"
        printf "%s\n" "CORE_MCU_LDFLAGS += -mcpu=$cpu"
        printf "%s\n" "CORE_MCU_CPPFLAGS += -mcpu=$cpu"
    } >> "$out_makefile"
fi

if [[ "$fpu" != "null" ]]
then
    {
        printf "%s\n" "CORE_MCU_FPU := $fpu"
        printf "%s\n" "CORE_MCU_LDFLAGS += -mfpu=$fpu"
        printf "%s\n" "CORE_MCU_CPPFLAGS += -mfpu=$fpu"
    } >> "$out_makefile"
fi

if [[ "$float_abi" != "null" ]]
then
    {
        printf "%s\n" "CORE_MCU_FLOAT_ABI := $float_abi"
        printf "%s\n" "CORE_MCU_LDFLAGS += -mfloat-abi=$float_abi"
        printf "%s\n" "CORE_MCU_CPPFLAGS += -mfloat-abi=$float_abi"
    } >> "$out_makefile"
fi

if [[ ("$external_freq" != "") && ("$external_freq" != "null") ]]
then
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_EXT_CLOCK_FREQ_MHZ=$external_freq" >> "$out_makefile"
fi

printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_CPU_FREQ_MHZ=$freq" >> "$out_makefile"

if [[ $($yaml_parser "$yaml_file" flash) != "null" ]]
then
    declare -i number_of_flash_pages
    flashStart=$($yaml_parser "$yaml_file" flash.flash-start)

    if [[ $($yaml_parser "$yaml_file" flash.pages) != "null" ]]
    then
        number_of_flash_pages=$($yaml_parser "$yaml_file" flash.pages --length)

        printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_TOTAL_FLASH_PAGES=$number_of_flash_pages" >> "$out_makefile"

        {
            printf "%s\n" "namespace {"
            printf "%s\n" "constexpr inline core::mcu::flash::flashPage_t FLASH_PAGE_DESCRIPTOR[CORE_MCU_TOTAL_FLASH_PAGES] = {"
        } >> "$out_header"

        flash_size=0

        for ((i=0; i<number_of_flash_pages; i++))
        do
            addressStart=$($yaml_parser "$yaml_file" flash.pages.["$i"].address)
            page_size=$($yaml_parser "$yaml_file" flash.pages.["$i"].size)
            ((flash_size+=page_size))

            {
                printf "%s\n" "{"
                printf "%s\n" ".address = $addressStart",
                printf "%s\n" ".size = $page_size",
                printf "%s\n" "},"
            } >> "$out_header"
        done

        {
            printf "%s\n" "};"
            printf "%s\n" "}"
        } >> "$out_header"

        {
            printf "%s\n" "constexpr uint32_t CORE_MCU_FLASH_PAGE_ADDR(size_t index) {"
            printf "%s\n" "return FLASH_PAGE_DESCRIPTOR[index].address; }"
            printf "%s\n" "constexpr uint32_t CORE_MCU_FLASH_PAGE_SIZE(size_t index) {"
            printf "%s\n" "return FLASH_PAGE_DESCRIPTOR[index].size; }"
        } >> "$out_header"
    else
        # all flash pages have common size
        page_size=$($yaml_parser "$yaml_file" flash.page-size)
        flash_size=$($yaml_parser "$yaml_file" flash.size)
        number_of_flash_pages=$((flash_size/page_size))

        {
            printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_TOTAL_FLASH_PAGES=$number_of_flash_pages"
            printf "%s\n" "CORE_MCU_FLASH_PAGE_SIZE_COMMON := $page_size"
            printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_FLASH_PAGE_SIZE_COMMON=$page_size"
        } >> "$out_makefile"

        {
            printf "%s\n" "constexpr uint32_t CORE_MCU_FLASH_PAGE_SIZE(size_t index) {"
            printf "%s\n" "return CORE_MCU_FLASH_PAGE_SIZE_COMMON; }"

            printf "%s\n" "constexpr uint32_t CORE_MCU_FLASH_PAGE_ADDR(size_t index) {"
            printf "%s\n" "return $flashStart + (CORE_MCU_FLASH_PAGE_SIZE_COMMON * index); }"
        } >> "$out_header"
    fi

    {
        printf "%s\n" "CORE_MCU_FLASH_SIZE := $flash_size"
        printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_FLASH_SIZE=$flash_size"
        printf "%s\n" "CORE_MCU_LDFLAGS += -Xlinker --defsym=CORE_MCU_FLASH_SIZE=$flash_size"
        printf "%s\n" "CORE_MCU_FLASH_START_ADDR := $flashStart"
        printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_FLASH_START_ADDR=$flashStart"
        printf "%s\n" "CORE_MCU_LDFLAGS += -Xlinker --defsym=CORE_MCU_FLASH_START_ADDR=$flashStart"
    } >> "$out_makefile"
fi

if [[ $($yaml_parser "$yaml_file" eeprom) != "null" ]]
then
    eeprom_size=$($yaml_parser "$yaml_file" eeprom.size)
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_EEPROM_SIZE=$eeprom_size" >> "$out_makefile"
fi

number_of_uart_interfaces=$($yaml_parser "$yaml_file" peripherals.uart)
number_of_i2c_interfaces=$($yaml_parser "$yaml_file" peripherals.i2c)

if [[ $($yaml_parser "$yaml_file" peripherals.custom-pins) == "true" ]]
then
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_CUSTOM_PERIPHERAL_PINS" >> "$out_makefile"
fi

{
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_MAX_UART_INTERFACES=${number_of_uart_interfaces}"
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_MAX_I2C_INTERFACES=${number_of_i2c_interfaces}"
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_ADC_MAX_VALUE=$((2 ** adc_bits - 1))"
    printf "%s\n" "CORE_MCU_DEFINES += CORE_MCU_UID_BITS=${uid_bits}"
} >> "$out_makefile"

if [[ $($yaml_parser "$yaml_file" define-symbols) != "null" ]]
then
    total_symbols=$($yaml_parser "$yaml_file" define-symbols --length)

    for ((i=0;i<total_symbols;i++))
    do
        symbol=$($yaml_parser "$yaml_file" define-symbols.["$i"])
        printf "%s\n" "CORE_MCU_DEFINES += $symbol" >> "$out_makefile"
    done
fi

if [[ $($yaml_parser "$yaml_file" include-dirs) != "null" ]]
then
    total_include_dirs=$($yaml_parser "$yaml_file" include-dirs --length)

    for ((i=0;i<total_include_dirs;i++))
    do
        dir=$($yaml_parser "$yaml_file" include-dirs.["$i"])
        printf "%s\n" "CORE_MCU_INCLUDE_DIRS += -I\"${script_dir}/../$dir\"" >> "$out_makefile"
    done
fi

if [[ $($yaml_parser "$yaml_file" sources) != "null" ]]
then
    total_sources=$($yaml_parser "$yaml_file" sources --length)

    for ((i=0;i<total_sources;i++))
    do
        source=$($yaml_parser "$yaml_file" sources.["$i"])
        printf "%s\n" "CORE_MCU_SOURCES += ${script_dir}/../$source" >> "$out_makefile"
    done
fi

if [[ $($yaml_parser "$yaml_file" ld-flags) != "null" ]]
then
    total_ld_flags=$($yaml_parser "$yaml_file" ld-flags --length)

    for ((i=0;i<total_ld_flags;i++))
    do
        flag=$($yaml_parser "$yaml_file" ld-flags.["$i"])
        printf "%s\n" "CORE_MCU_LDFLAGS += $flag" >> "$out_makefile"
    done
fi

if [[ $($yaml_parser "$yaml_file" ld-libs) != "null" ]]
then
    total_ld_libs=$($yaml_parser "$yaml_file" ld-libs --length)

    for ((i=0;i<total_ld_libs;i++))
    do
        lib=$($yaml_parser "$yaml_file" ld-libs.["$i"])
        printf "%s\n" "CORE_MCU_LDLIBS += ${script_dir}/../$lib" >> "$out_makefile"
    done
fi

if [[ $gen_usb -eq 1 ]]
then
    if [[ $($yaml_parser "$yaml_file" usb) != "null" ]]
    then
        if [[ $($yaml_parser "$yaml_file" usb.define-symbols) != "null" ]]
        then
            total_symbols=$($yaml_parser "$yaml_file" usb.define-symbols --length)

            for ((i=0;i<total_symbols;i++))
            do
                symbol=$($yaml_parser "$yaml_file" usb.define-symbols.["$i"])
                printf "%s\n" "CORE_MCU_DEFINES += $symbol" >> "$out_makefile"
            done
        fi

        if [[ $($yaml_parser "$yaml_file" usb.include-dirs) != "null" ]]
        then
            total_include_dirs=$($yaml_parser "$yaml_file" usb.include-dirs --length)

            for ((i=0;i<total_include_dirs;i++))
            do
                dir=$($yaml_parser "$yaml_file" usb.include-dirs.["$i"])
                printf "%s\n" "CORE_MCU_INCLUDE_DIRS += -I\"${script_dir}/../$dir\"" >> "$out_makefile"
            done
        fi

        if [[ $($yaml_parser "$yaml_file" usb.sources) != "null" ]]
        then
            total_sources=$($yaml_parser "$yaml_file" usb.sources --length)

            for ((i=0;i<total_sources;i++))
            do
                source=$($yaml_parser "$yaml_file" usb.sources.["$i"])
                printf "%s\n" "CORE_MCU_SOURCES += ${script_dir}/../$source" >> "$out_makefile"
            done
        fi
    fi
fi

if [[ $($yaml_parser "$yaml_file" dl-deps) != "null" ]]
then
    command -v wget >/dev/null 2>&1 || { echo "wget is not installed"; exit 1; }

    total_deps=$($yaml_parser "$yaml_file" dl-deps --length)
    dep_dir="${script_dir}"/../src/core/deps/"${mcu}"
    mkdir -p "$dep_dir"

    for ((i=0;i<total_deps;i++))
    do
        dep=$($yaml_parser "$yaml_file" dl-deps.["$i"])
        filename=$(basename "$dep")

        if [[ ! -f "$dep_dir/$filename" ]]
        then
            echo "Downloading dependency for $mcu: $dep"
            wget -q --show-progress "$dep" -P "$dep_dir"
            ext="${dep##*.}"

            case "$ext" in
                "zip")
                    command -v unzip >/dev/null 2>&1 || { echo "unzip is not installed"; exit 1; }
                    echo "Extracting..."
                    unzip -q "$dep_dir/$filename" -d "$dep_dir"
                    ;;

                    *)
                    ;;
            esac
        fi
    done
fi