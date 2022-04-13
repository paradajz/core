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
#include "core/src/arch/avr/common/Atomic.h"
#include "IO.h"

namespace core::mcu::spi
{
    constexpr inline uint8_t SPI_CLOCK_MASK   = 0x03;
    constexpr inline uint8_t SPI_2XCLOCK_MASK = 0x01;
    constexpr inline uint8_t SPI_MODE_MASK    = 0x0C;

    enum class mode_t : uint8_t
    {
        MODE_0 = 0x00,
        MODE_1 = 0x04,
        MODE_2 = 0x08,
        MODE_3 = 0x0C
    };

    enum class clockDiv_t : uint8_t
    {
        DIV_2   = 0x04,
        DIV_4   = 0x00,
        DIV_8   = 0x05,
        DIV_16  = 0x01,
        DIV_32  = 0x06,
        DIV_64  = 0x02,
        DIV_128 = 0x03
    };

    enum class bitOrder_t : uint8_t
    {
        LSB,
        MSB
    };

    inline void init()
    {
        CORE_MCU_ATOMIC_SECTION
        {
            // SS
            CORE_MCU_IO_INIT(PORTB, 0, core::mcu::io::pinMode_t::OUTPUT);
            CORE_MCU_IO_SET_HIGH(PORTB, 0);

            SPCR |= (1 << MSTR);
            SPCR |= (1 << SPE);

            // clock
            CORE_MCU_IO_INIT(PORTB, 1, core::mcu::io::pinMode_t::OUTPUT);
            // mosi
            CORE_MCU_IO_INIT(PORTB, 2, core::mcu::io::pinMode_t::OUTPUT);
        }
    }

    inline uint8_t spiTransfer(uint8_t data)
    {
        SPDR = data;

        while (!(SPSR & (1 << SPIF)))
        {
            ;
        }

        return SPDR;
    }

    inline void setBitOrder(bitOrder_t bitOrder)
    {
        if (bitOrder == bitOrder_t::LSB)
        {
            SPCR |= _BV(DORD);
        }
        else
        {
            SPCR &= ~(_BV(DORD));
        }
    }

    inline void setClockDivider(clockDiv_t clockDiv)
    {
        SPCR = (SPCR & ~SPI_CLOCK_MASK) | (static_cast<uint8_t>(clockDiv) & SPI_CLOCK_MASK);
        SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((static_cast<uint8_t>(clockDiv) >> 2) & SPI_2XCLOCK_MASK);
    }

    inline void setDataMode(mode_t dataMode)
    {
        SPCR = (SPCR & ~SPI_MODE_MASK) | static_cast<uint8_t>(dataMode);
    }
}    // namespace core::mcu::spi
