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

#if !__has_include(<MCU.h>)
// already defined in generated MCU, define only if it doesn't exist
#define CORE_MCU_ADC_MAX_VALUE 1023
#endif

namespace core::mcu::adc
{
    enum class prescaler_t : uint8_t
    {
        P128 = 128,
        P64  = 64,
        P32  = 32,
        P16  = 16
    };

    enum class vRef_t : uint8_t
    {
        AREF     = 0,
        AVCC     = 1,
        INT_2V56 = 2,
        INT_1V1  = 3
    };

    /// Structure holding ADC prescaler and voltage reference settings.
    struct conf_t
    {
        prescaler_t prescaler;
        vRef_t      vref;
    };

    void     init(conf_t configuration) __attribute__((weak));
    uint16_t read(uint32_t channel) __attribute__((weak));
    void     startItConversion() __attribute__((weak));
    void     setChannel(uint32_t channel) __attribute__((weak));
    void     disconnectDigitalIn(uint32_t channel) __attribute__((weak));
}    // namespace core::mcu::adc