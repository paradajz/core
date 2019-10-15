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

#ifndef __CORE_STUB_IO
#define __CORE_STUB_IO

#include <inttypes.h>

#define CORE_IO_CONFIG(port, pin, mode)
#define CORE_IO_SET_LOW(port, pin)
#define CORE_IO_SET_HIGH(port, pin)
#define CORE_IO_READ(port, pin) 0

namespace core
{
    namespace io
    {
        ///
        /// \brief Structure used to define single MCU pin.
        ///
        typedef struct
        {
            volatile uint16_t port;
            uint16_t          pin;
        } mcuPin_t;

        ///
        /// \brief Structure used to define single PWM channel.
        ///
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

#endif