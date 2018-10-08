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

#include <inttypes.h>

///
/// \ingroup coreHALavrSPI
/// @{

#define SPI_CLOCK_MASK      0x03
#define SPI_2XCLOCK_MASK    0x01
#define SPI_CLOCK_DIV4      0x00
#define SPI_CLOCK_DIV16     0x01
#define SPI_CLOCK_DIV64     0x02
#define SPI_CLOCK_DIV128    0x03
#define SPI_CLOCK_DIV2      0x04
#define SPI_CLOCK_DIV8      0x05
#define SPI_CLOCK_DIV32     0x06

#define SPI_MODE0           0x00
#define SPI_MODE1           0x04
#define SPI_MODE2           0x08
#define SPI_MODE3           0x0C

#define SPI_MODE_MASK       0x0C

#define LSBFIRST            0
#define MSBFIRST            1

inline static void setBitOrder(uint8_t bitOrder)
{
    if (bitOrder == LSBFIRST)
        SPCR |= _BV(DORD);
    else
        SPCR &= ~(_BV(DORD));
}

inline static void setClockDivider(uint8_t clockDiv)
{
    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clockDiv & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clockDiv >> 2) & SPI_2XCLOCK_MASK);
}

inline static void setDataMode(uint8_t dataMode)
{
    SPCR = (SPCR & ~SPI_MODE_MASK) | dataMode;
}

///
/// \brief Initialize SPI peripheral.
///
void spiInit();

///
/// \brief Transfers one byte of data using SPI peripheral.
/// @param [in] data    Data to be transfered over SPI.
///
uint8_t spiTransfer(uint8_t data);

/// @}