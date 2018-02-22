/*
    Copyright 2017-2018 Igor Petrovic

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

///
/// \brief Helper macros for easier byte/bit manipulation.
/// \defgroup coreGeneralBitManipulation Bit manipulation
/// \ingroup coreGeneral
/// @{

#define BIT_READ(value, bit)             (((value) >> (bit)) & 0x01)
#define BIT_SET(value, bit)              ((value) |= (1UL << (bit)))
#define BIT_CLEAR(value, bit)            ((value) &= ~(1UL << (bit)))
#define BIT_WRITE(value, bit, bitvalue)  (bitvalue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))
#define BYTE_INVERT(value)               ((value) ^ 0xFF)
#define BYTE_LOW(value)                  ((value) & 0xFF)
#define BYTE_HIGH(value)                 (((value) >> 8) & 0xFF)

/// @}