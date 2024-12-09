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

#ifndef CORE_MCU_STUB

#include <inttypes.h>
#include "core/mcu.h"

namespace core::mcu::adc
{
    /// Structure holding ADC prescaler and voltage reference settings.
    struct conf_t
    {
        uint32_t prescaler   = 1;
        bool     externalRef = false;

        // accepted values are:
        // 30: 3.0 volts
        // 33: 3.3 volts
        // 50: 5.0 volts
        uint8_t voltage = 33;

        conf_t() = default;
    };

    void     init(conf_t configuration);
    void     initPin(core::mcu::io::pin_t pin);
    void     enableIt(uint8_t priority = 0);
    void     disableIt();
    void     startItConversion();
    void     setActivePin(core::mcu::io::pin_t pin);
    uint16_t read(core::mcu::io::pin_t pin);
}    // namespace core::mcu::adc

#else
#include "core/arch/stub/adc.h"
#endif