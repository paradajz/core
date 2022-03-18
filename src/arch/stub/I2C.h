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

#ifndef __CORE_STUB_I2C
#define __CORE_STUB_I2C

#include <inttypes.h>

#define TWCR            (*(volatile uint32_t*)(1))
#define TWDR            (*(volatile uint32_t*)(1))
#define TW_STATUS       (*(volatile uint32_t*)(1))
#define TWBR            (*(volatile uint32_t*)(1))
#define TWSR            (*(volatile uint32_t*)(1))
#define TW_MT_SLA_ACK   0
#define TWINT           1
#define TWSTA           2
#define TWIE            3
#define TWEN            3
#define TW_MR_SLA_NACK  4
#define TW_MT_SLA_NACK  5
#define TW_MT_DATA_NACK 6
#define TW_MT_ARB_LOST  7
#define TW_BUS_ERROR    8
#define TW_NO_INFO      9
#define TW_MT_DATA_ACK  10
#define TWEA            11
#define TW_START        12
#define TW_REP_START    13
#define TWSTO           14
#define TW_MR_SLA_ACK   15

namespace core::i2c
{
    enum class transferType_t : uint8_t
    {
        WRITE,
        READ
    };

    void enable() __attribute__((weak));
    void disable(bool force) __attribute__((weak));
    bool startComm(uint8_t address, transferType_t type) __attribute__((weak));
    void stopComm() __attribute__((weak));
    bool write(uint8_t data) __attribute__((weak));
    void read(uint8_t& data) __attribute__((weak));
}    // namespace core::i2c

#endif