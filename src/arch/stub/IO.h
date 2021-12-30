/*
    Copyright 2017-2021 Igor Petrovic

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
        using gpioPinPort_t  = volatile uint8_t*;
        using gpioPinIndex_t = uint8_t;

        enum class pinMode_t : uint32_t
        {
            input,
            output,
            outputPP,
            outputOD,
            alternatePP,
            alternateOD,
            analog,
            itRising,
            itFalling,
            itRisingFalling
        };

        enum class pullMode_t : uint32_t
        {
            none,
            up,
            down
        };

        enum class gpioSpeed_t : uint32_t
        {
            low,
            medium,
            high,
            veryHigh
        };

        typedef struct
        {
            gpioPinPort_t  port;
            gpioPinIndex_t index;
            pinMode_t      mode;
            pullMode_t     pull;
            gpioSpeed_t    speed;
            uint32_t       alternate;
        } mcuPin_t;

        typedef struct
        {
            volatile uint8_t* timer;
            volatile uint8_t* compareL;
            volatile uint8_t* compareH;
            uint8_t           channel;
        } pwmChannel_t;

        void pwmOff(pwmChannel_t pwm) __attribute__((weak));
        void pwmOn(pwmChannel_t pwm, uint16_t intensity) __attribute__((weak));
    }    // namespace io
}    // namespace core

#define CORE_IO_CONFIG(port, pin, mode)
#define CORE_IO_SET_LOW(port, pin)
#define CORE_IO_SET_HIGH(port, pin)
#define CORE_IO_READ(port, pin)   0
#define CORE_IO_PORT(port)        0
#define CORE_IO_PORT_INDEX(index) index
#define CORE_IO_MCU_PIN_DEF(...) \
    {                            \
        .port = 0, .index = 0    \
    }

#endif