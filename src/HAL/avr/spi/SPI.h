/*
    Copyright 2017-2019 Igor Petrovic

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

///
/// \ingroup coreHALavrSPI
/// @{

namespace SPI
{
    enum class mode_t : uint8_t
    {
        mode0 = 0x00,
        mode1 = 0x04,
        mode2 = 0x08,
        mode3 = 0x0C
    };

    enum class clockDiv_t : uint8_t
    {
        div2 = 0x04,
        div4 = 0x00,
        div8 = 0x05,
        div16 = 0x01,
        div32 = 0x06,
        div64 = 0x02,
        div128 = 0x03
    };

    enum class bitOrder_t : uint8_t
    {
        lsb,
        msb
    };

    void init();
    uint8_t spiTransfer(uint8_t data);
    void setBitOrder(bitOrder_t bitOrder);
    void setClockDivider(clockDiv_t clockDiv);
    void setDataMode(mode_t dataMode);
};

/// @}