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

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include <inttypes.h>
#include "core/src/MCU.h"

namespace core::mcu::i2c
{
    class Config
    {
        public:
        enum class clockSpeed_t : uint32_t
        {
            S100K = 100000,
            S400K = 400000
        };

        struct pins_t
        {
            core::mcu::io::pin_t sda;
            core::mcu::io::pin_t scl;
        };

        Config(clockSpeed_t clockSpeed,
               pins_t       pins)
            : clockSpeed(clockSpeed)
            , pins(pins)
        {}

        Config(clockSpeed_t clockSpeed)
            : clockSpeed(clockSpeed)
            , pins({})
        {}

        Config()
            : clockSpeed(clockSpeed_t::S100K)
            , pins({})
        {}

        clockSpeed_t clockSpeed;
        pins_t       pins;
    };

    bool init(uint8_t channel, const Config& config);
    bool deInit(uint8_t channel);
    bool write(uint8_t channel, uint8_t address, uint8_t* buffer, size_t size);
    bool deviceAvailable(uint8_t channel, uint8_t address);
}    // namespace core::mcu::i2c