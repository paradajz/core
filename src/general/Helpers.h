/*
    Copyright 2017-2021 Igor Petrovic

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

#ifndef __CORE_GENERAL_HELPERS
#define __CORE_GENERAL_HELPERS

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

///
/// \brief Calculates size of array.
/// @param [in] x   Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

///
/// \brief Calculates size of char array.
/// @param [in] x   Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE_CHAR(x) (sizeof(x) / sizeof(x[0]) - 1)

///
/// \brief Constrains input value to defined range.
/// @param [in] input   Input value.
/// @param [in] low     Lowest possible value.
/// @param [in] high    Largest possible value.
///
#define CONSTRAIN(input, low, high) ((input) < (low) ? (low) : ((input) > (high) ? (high) : (input)))

///
/// \brief Definitions for active pin/signal logic.
/// @{

#define ACTIVE_HIGH 1
#define ACTIVE_LOW  0

/// @}

///
/// \brief Used to retrieve least significant byte in a single word (2 bytes).
/// @param [in] value   Single word.
/// \returns LSB from given word.
///
#define LSB_WORD(value) (value & 0xFF)

///
/// \brief Used to retrieve most significant byte in a single word (2 bytes).
/// @param [in] value   Single word.
/// \returns MSB from given word.
///
#define MSB_WORD(value) (value >> 8 & 0xFF)

///
/// \brief Used to construct word from given most and least significant byte.
/// @param [in] lsb LSB in a word to construct.
/// @param [in] msb MSB in a word to construct.
/// \returns Word from given LSB and MSB.
///
#define GET_WORD(lsb, msb) (msb << 8 | lsb)

///
/// \brief Helper macros for easier byte/bit manipulation.
/// \defgroup coreGeneralBitManipulation Bit manipulation
/// \ingroup coreGeneral
/// @{

#define BIT_READ(value, bit)            (((value) >> (bit)) & 0x01)
#define BIT_SET(value, bit)             ((value) |= (1UL << (bit)))
#define BIT_CLEAR(value, bit)           ((value) &= ~(1UL << (bit)))
#define BIT_WRITE(value, bit, bitvalue) ((bitvalue) ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))
#define BYTE_INVERT(value)              ((value) ^ 0xFF)
#define BYTE_LOW(value)                 ((value) & (0xFF))
#define BYTE_HIGH(value)                (((value) >> 8) & 0xFF)

/// @}

// on AVRs, it is common to place strings or other constant data in program memory
// however, that concept doesn't exist on other platforms
// in that case, allow compiling the AVR code by re-defining certain functions/macros
#ifdef __AVR__
#include <avr/pgmspace.h>
#define STRING_PROGMEM_ARRAY(name) const char* const name[] PROGMEM
#define READ_PROGMEM_ARRAY(string) (PGM_P) pgm_read_word(&(string))
#define READ_PROGMEM_BYTE(address) pgm_read_byte(&address)
#define READ_PROGMEM_WORD(address) pgm_read_word(&address)
#else
#define PGM_P                      const char*
#define strcpy_P                   strcpy
#define STRING_PROGMEM_ARRAY(name) const char* name[]
#define READ_PROGMEM_BYTE(address) address
#define READ_PROGMEM_WORD(address) address
#define PROGMEM
#define READ_PROGMEM_ARRAY(string) (const char*)((string))
#endif

#ifdef __cplusplus
namespace core
{
    namespace misc
    {
        ///
        /// \brief Used to map one range of numbers to another one.
        ///
        template<typename T>
        inline T mapRange(T x, T in_min, T in_max, T out_min, T out_max)
        {
            // don't bother with mapping in this case
            if ((in_min == out_min) && (in_max == out_max))
                return x;

            // smaller input range to larger output range
            if ((in_max - in_min) > (out_max - out_min))
                return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;

            // larger input range to smaller output range
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        };

        ///
        /// \brief Checks number of digits for requested value.
        /// @param[in] number   Number for which number if digits is being calculated.
        /// \returns Number of digits.
        ///
        inline uint8_t getNumberOfDigits(int32_t number)
        {
            // make sure negative numbers are processed correctly
            number = abs(number);

            if (number < 10)
                return 1;

            if (number < 100)
                return 2;

            if (number < 1000)
                return 3;

            if (number < 10000)
                return 4;

            if (number < 1000000)
                return 6;

            if (number < 10000000)
                return 7;

            if (number < 100000000)
                return 8;

            if (number < 1000000000)
                return 9;

            return 10;    // max size
        }

        inline uint8_t maskToIndex(uint32_t mask)
        {
            if (!mask)
                return 0;

            for (size_t i = 0; i < 32; i++)
            {
                if ((mask >> i) & 0x01)
                {
                    return i;
                }
            }

            return 0;
        }
    }    // namespace misc
}    // namespace core
#endif

#endif