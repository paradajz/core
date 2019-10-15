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

#ifndef __CORE_STUB_I2C
#define __CORE_STUB_I2C

#include <inttypes.h>

namespace core
{
    namespace i2c
    {
        enum class transferType_t : uint8_t
        {
            write,
            read
        };

        void enable() __attribute__((weak));
        void disable(bool force) __attribute__((weak));
        bool startComm(uint8_t address, transferType_t type) __attribute__((weak));
        void stopComm() __attribute__((weak));
        bool write(uint8_t data) __attribute__((weak));
        void read(uint8_t& data) __attribute__((weak));
    }    // namespace i2c
}    // namespace core

#endif