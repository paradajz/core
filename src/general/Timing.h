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
#ifdef __AVR__
#include <util/atomic.h>
#include <util/delay.h>
#endif

///
/// \ingroup coreGeneral
/// @{

///
/// \brief Definition of variable holding current MCU run time.
/// Must be implemented externally in order to use rTimeMs() function correctly.
///
extern volatile uint32_t rTime_ms;

///
/// \brief Returns amount of time MCU has been running in milliseconds.
/// Actual incrementation of rTime_ms must be done externally.
/// \returns Runtime in milliseconds.
///
inline uint32_t rTimeMs()
{
    uint32_t _rTime_mS;

    #ifdef __AVR__
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    #endif
    {
        _rTime_mS = rTime_ms;
    }

    return _rTime_mS;
}

///
/// \brief Delays for desired time interval in milliseconds.
/// This function makes use of built-in _delay_ms function. Function is called repeatedly with argument 1 until
/// ms parameter reaches 0, since _delay_ms accepts only constant known at compile-time.
/// @param [in] ms  Delay time in milliseconds.
///
inline void wait_ms(uint32_t ms)
{
    while (ms--)
    {
        #ifdef __AVR__
        _delay_ms(1);
        #endif
    }
}


/// @}