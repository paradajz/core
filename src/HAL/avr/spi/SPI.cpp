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

#include <util/atomic.h>
#include "SPI.h"
#include "../PinManipulation.h"

#define SPI_CLOCK_MASK      0x03
#define SPI_2XCLOCK_MASK    0x01
#define SPI_MODE_MASK       0x0C

namespace SPI
{
    void spiInit()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            //SS, at90usb1286
            setOutput(PORTB, 0);
            setHigh(PORTB, 0);

            SPCR |= (1<<MSTR);
            SPCR |= (1<<SPE);

            //clock, at90usb1286
            setOutput(PORTB, 1);
            //mosi, at90usb1286
            setOutput(PORTB, 2);
        }
    }

    uint8_t spiTransfer(uint8_t data)
    {
        SPDR = data;
        while (!(SPSR & (1<<SPIF)));
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
}