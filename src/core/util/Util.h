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

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#ifndef CORE_MCU_STUB
#ifdef CORE_MCU_ARCH_AVR
#include <avr/pgmspace.h>
#endif
#endif

namespace core::util
{
    /// Constrains input value to defined range.
    /// param [in]: input   Input value.
    /// param [in]: low     Lowest possible value.
    /// param [in]: high    Largest possible value.
    template<typename T>
    constexpr T CONSTRAIN(T input, T low, T high)
    {
        return input < low ? low : input > (high) ? high
                                                  : input;
    }

    /// Used to retrieve least significant byte in a single word (2 bytes).
    /// param [in]: value   Single word.
    /// returns: LSB from given word.
    constexpr uint8_t LSB_U16(uint16_t value)
    {
        return value & 0xFF;
    }

    /// Used to retrieve most significant byte in a single word (2 bytes).
    /// param [in]: value   Single word.
    /// returns: MSB from given word.
    constexpr uint8_t MSB_U16(uint16_t value)
    {
        return value >> 8 & 0xFF;
    }

    /// Used to map one range of numbers to another one.
    template<typename T>
    constexpr T MAP_RANGE(T x, T inMin, T inMax, T outMin, T outMax)
    {
        // don't bother with mapping in this case
        if ((inMin == outMin) && (inMax == outMax))
        {
            return x;
        }

        // smaller input range to larger output range
        if ((inMax - inMin) > (outMax - outMin))
        {
            return (x - inMin) * (outMax - outMin + 1) / (inMax - inMin + 1) + outMin;
        }

        // larger input range to smaller output range
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    };

    constexpr uint8_t MASK_TO_INDEX(uint32_t mask)
    {
        uint8_t index = 0;

        while (mask >>= 1)
        {
            index++;
        }

        return index;
    }

    inline uint16_t XMODEM(uint16_t crc, uint8_t data)
    {
        crc = crc ^ (static_cast<uint16_t>(data) << 8);

        for (int i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc <<= 1;
            }
        }

        return crc;
    }

    template<typename T>
    constexpr inline bool BIT_READ(T value, size_t bit)
    {
        return (value >> bit) & static_cast<T>(1);
    }

    template<typename T>
    constexpr inline void BIT_SET(T& value, size_t bit)
    {
        value |= (static_cast<T>(1) << bit);
    }

    template<typename T>
    constexpr inline void BIT_CLEAR(T& value, size_t bit)
    {
        value &= ~(static_cast<T>(1) << bit);
    }

    template<typename T>
    constexpr inline void BIT_WRITE(T& value, size_t bit, bool bitValue)
    {
        bitValue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit);
    }
}    // namespace core::util

// on AVRs, it is common to place strings or other constant data in program memory
// however, that concept doesn't exist on other platforms
// in that case, allow compiling the AVR code by re-defining certain functions/macros
#if defined(CORE_MCU_ARCH_AVR) && !defined(CORE_MCU_STUB)
#define CORE_UTIL_READ_PROGMEM_ARRAY(string) (PGM_P) pgm_read_word(&(string))
#define CORE_UTIL_READ_PROGMEM_BYTE(address) pgm_read_byte(&address)
#define CORE_UTIL_READ_PROGMEM_WORD(address) pgm_read_word(&address)
#else
#define PROGMEM
#define PGM_P                                const char*
#define strcpy_P                             strcpy
#define CORE_UTIL_READ_PROGMEM_ARRAY(string) (const char*)((string))
#define CORE_UTIL_READ_PROGMEM_BYTE(address) address
#define CORE_UTIL_READ_PROGMEM_WORD(address) address
#endif

#ifdef CORE_MCU_STUB
#include "core/arch/stub/Util.h"
#else
#ifdef CORE_MCU_ARCH_AVR
#include "core/arch/avr/common/Util.h"
#elif defined(CORE_MCU_ARCH_ARM)
#include "core/arch/arm/common/Util.h"
#endif
#endif
