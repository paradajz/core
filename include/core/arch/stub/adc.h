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
#include "io.h"

namespace core::mcu::adc
{
    struct conf_t
    {
        uint32_t prescaler   = 1;
        bool     externalRef = false;
        uint8_t  voltage     = 33;

        conf_t() = default;
    };

    inline void init(conf_t configuration)
    {}

    inline void initPin(core::mcu::io::pin_t pin)
    {}

    inline void enableIt(uint8_t priority = 0)
    {}

    inline void disableIt()
    {}

    inline void startItConversion()
    {}

    inline void setActivePin(core::mcu::io::pin_t pin)
    {}

    inline uint16_t read(core::mcu::io::pin_t pin)
    {
        return 0;
    }

}    // namespace core::mcu::adc