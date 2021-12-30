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

#ifndef __CORE_AVR_SPI
#define __CORE_AVR_SPI

#include <inttypes.h>
#include <util/atomic.h>
#include "IO.h"

#define SPI_CLOCK_MASK   0x03
#define SPI_2XCLOCK_MASK 0x01
#define SPI_MODE_MASK    0x0C

namespace core
{
    namespace spi
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
            div2   = 0x04,
            div4   = 0x00,
            div8   = 0x05,
            div16  = 0x01,
            div32  = 0x06,
            div64  = 0x02,
            div128 = 0x03
        };

        enum class bitOrder_t : uint8_t
        {
            lsb,
            msb
        };

        void init()
        {
            ATOMIC_SECTION
            {
                // SS
                setOutput(PORTB, 0);
                setHigh(PORTB, 0);

                SPCR |= (1 << MSTR);
                SPCR |= (1 << SPE);

                // clock
                setOutput(PORTB, 1);
                // mosi
                setOutput(PORTB, 2);
            }
        }

        uint8_t spiTransfer(uint8_t data)
        {
            SPDR = data;
            while (!(SPSR & (1 << SPIF)))
                ;
            return SPDR;
        }

        void setBitOrder(bitOrder_t bitOrder)
        {
            if (bitOrder == bitOrder_t::lsb)
                SPCR |= _BV(DORD);
            else
                SPCR &= ~(_BV(DORD));
        }

        void setClockDivider(clockDiv_t clockDiv)
        {
            SPCR = (SPCR & ~SPI_CLOCK_MASK) | ((uint8_t)clockDiv & SPI_CLOCK_MASK);
            SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | (((uint8_t)clockDiv >> 2) & SPI_2XCLOCK_MASK);
        }

        void setDataMode(mode_t dataMode)
        {
            SPCR = (SPCR & ~SPI_MODE_MASK) | (uint8_t)dataMode;
        }
    }    // namespace spi
}    // namespace core

#endif