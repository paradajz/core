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

#include <vector>
#include "IO.h"

namespace core::mcu::peripherals
{
    class Descriptor
    {
        public:
        virtual std::vector<core::mcu::io::pin_t> pins()         = 0;
        virtual void*                             interface()    = 0;
        virtual uint8_t                           irqn()         = 0;
        virtual void                              enableClock()  = 0;
        virtual void                              disableClock() = 0;
    };

    constexpr uint32_t adcChannel(const core::mcu::io::pin_t& pin)
    {
        return pin.index;
    }

    inline Descriptor* uartDescriptor(uint8_t channel)
    {
        return nullptr;
    }

    inline Descriptor* i2cDescriptor(uint8_t channel)
    {
        return nullptr;
    }

    inline Descriptor* usbDescriptor()
    {
        return nullptr;
    }

    inline bool uartChannel(void* interface, uint8_t& channel)
    {
        return false;
    }

    inline bool i2cChannel(void* interface, uint8_t& channel)
    {
        return false;
    }
}    // namespace core::mcu::peripherals
