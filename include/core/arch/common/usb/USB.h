/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <inttypes.h>
#include "lufa/Descriptors.h"

#define CORE_MCU_USB_STRING(string) u##string

// avoid lufa overriding symbols below if included
#undef CONF_DESC_POWER_MA
#undef CONF_DESC_ATTR_RESERVED
#undef ENDPOINT_DIR_OUT
#undef ENDPOINT_DIR_IN
#undef ENDPOINT_TYPE_CONTROL
#undef ENDPOINT_TYPE_ISOCHRONOUS
#undef ENDPOINT_TYPE_BULK
#undef ENDPOINT_TYPE_INTERRUPT
#undef ENDPOINT_ATTR_NO_SYNC
#undef ENDPOINT_USAGE_DATA
#undef NO_DESCRIPTOR
#undef LANGUAGE_ID_ENG
#undef STRING_LEN
#undef VERSION_BCD

namespace core::mcu::usb
{
    constexpr inline uint8_t CONF_DESC_POWER_MA(uint8_t mA)
    {
        return mA >> 1;
    }

    constexpr inline uint8_t  CONF_DESC_ATTR_RESERVED   = 0x80;
    constexpr inline uint8_t  ENDPOINT_DIR_OUT          = 0x00;
    constexpr inline uint8_t  ENDPOINT_DIR_IN           = 0x80;
    constexpr inline uint8_t  ENDPOINT_TYPE_CONTROL     = 0x00;
    constexpr inline uint8_t  ENDPOINT_TYPE_ISOCHRONOUS = 0x01;
    constexpr inline uint8_t  ENDPOINT_TYPE_BULK        = 0x02;
    constexpr inline uint8_t  ENDPOINT_TYPE_INTERRUPT   = 0x03;
    constexpr inline uint8_t  ENDPOINT_ATTR_NO_SYNC     = 0 << 2;
    constexpr inline uint8_t  ENDPOINT_USAGE_DATA       = 0 << 4;
    constexpr inline uint8_t  NO_DESCRIPTOR             = 0;
    constexpr inline uint16_t LANGUAGE_ID_ENG           = 0x0409;

    constexpr inline uint16_t STRING_LEN(uint16_t characters)
    {
        return sizeof(core::mcu::usb::descriptorHeader_t) + (characters << 1);
    }

    constexpr inline uint16_t VERSION_BCD(uint8_t major, uint8_t minor, uint8_t revision)
    {
        return (((major & 0xFF) << 8) |
                ((minor & 0x0F) << 4) |
                (revision & 0x0F));
    }
}    // namespace core::mcu::usb

#define CORE_MCU_USB_STRING_DESCRIPTOR_ARRAY(...)                                                     \
    {                                                                                                 \
        {                                                                                             \
            .size = sizeof(core::mcu::usb::descriptorHeader_t) + sizeof((uint16_t[]){ __VA_ARGS__ }), \
            .type = core::mcu::usb::DESC_TYPE_STRING                                                  \
        },                                                                                            \
        {                                                                                             \
            __VA_ARGS__                                                                               \
        }                                                                                             \
    }

#define CORE_MCU_USB_STRING_DESCRIPTOR(String)                                         \
    {                                                                                  \
        {                                                                              \
            .size = sizeof(core::mcu::usb::descriptorHeader_t) + (sizeof(String) - 2), \
            .type = core::mcu::usb::DESC_TYPE_STRING                                   \
        },                                                                             \
        {                                                                              \
            String                                                                     \
        }                                                                              \
    }
