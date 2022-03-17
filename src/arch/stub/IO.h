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

#ifndef __CORE_STUB_IO
#define __CORE_STUB_IO

#include <inttypes.h>

namespace core
{
    namespace io
    {
        using pinPort_t   = uint8_t;
        using pinIndex_t  = uint8_t;
        using portWidth_t = pinIndex_t;

        enum class pinMode_t : uint32_t
        {
            INPUT,
            OUTPUT,
            OUTPUT_PP,
            OUTPUT_OD,
            ALTERNATE_PP,
            ALTERNATE_OD,
            ANALOG,
            IT_RISING,
            IT_FALLING,
            IT_RISING_FALLING
        };

        enum class pullMode_t : uint32_t
        {
            NONE,
            UP,
            DOWN
        };

        enum class gpioSpeed_t : uint32_t
        {
            LOW,
            MEDIUM,
            HIGH,
            VERY_HIGH
        };

        struct mcuPin_t
        {
            pinPort_t   port;
            pinIndex_t  index;
            pinMode_t   mode;
            pullMode_t  pull;
            gpioSpeed_t speed;
            uint32_t    alternate;
        };

    }    // namespace io
}    // namespace core

#define CORE_IO_SET_LOW(port, index)
#define CORE_IO_SET_HIGH(port, index)
#define CORE_IO_SET_PORT_STATE(port, state)
#define CORE_IO_READ(port, index)   0
#define CORE_IO_READ_OUT_PORT(port) 0
#define CORE_IO_READ_IN_PORT(port)  0
#define CORE_IO_SET_STATE(port, index, state)
#define CORE_IO_TOGGLE(port, pin)
#define CORE_IO_PIN_PORT_DEF(port)    0
#define CORE_IO_PIN_INDEX_DEF(index)  0
#define CORE_IO_MCU_PIN_PORT(mcuPin)  0
#define CORE_IO_MCU_PIN_INDEX(mcuPin) 0
#define CORE_IO_INIT(port, index, mode)
#define CORE_IO_MCU_PIN_VAR(_port, _index) \
    {                                      \
        .port  = _port,                    \
        .index = _index                    \
    }
#endif