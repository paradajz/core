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

#ifndef __CORE_GENERAL_TIMING
#define __CORE_GENERAL_TIMING

#include <inttypes.h>
#ifdef __AVR__
#include <util/delay.h>
#elif defined __STM32__
#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F401xE) || defined(STM32F411xE)
#include "stm32f4xx_hal.h"
#elif defined(STM32F031x6)
#include "stm32f0xx_hal.h"
#endif
#endif
#include "Atomic.h"

namespace core
{
    namespace timing
    {
        namespace detail
        {
            ///
            /// \brief Definition of variable holding current MCU run time in milliseconds.
            /// Must be implemented externally in order to use core::timing::currentRunTimeMs() function correctly.
            ///
            extern volatile uint32_t rTime_ms;

            ///
            /// \brief Definition of variable holding current MCU run time in microseconds.
            /// Must be implemented externally in order to use core::timing::currentRunTimeUs() function correctly.
            ///
            extern volatile uint32_t rTime_us;
        }    // namespace detail

        ///
        /// \brief Delays for desired time interval in milliseconds.
        /// This function makes use of built-in _delay_ms function. Function is called repeatedly with argument 1 until
        /// ms parameter reaches 0, since _delay_ms accepts only constant known at compile-time.
        /// @param [in] ms  Delay time in milliseconds.
        ///
        inline void waitMs(uint32_t ms)
        {
#ifdef __AVR__
            while (ms--)
            {
                _delay_ms(1);
            }
#elif defined __STM32__
                HAL_Delay(ms);
#endif
        }

        ///
        /// \brief Returns amount of time MCU has been running in milliseconds.
        /// Actual incrementation of rTime_ms must be done externally.
        /// \returns Runtime in milliseconds.
        ///
        inline uint32_t currentRunTimeMs()
        {
            uint32_t _rTime_mS;

            ATOMIC_SECTION
            {
                _rTime_mS = detail::rTime_ms;
            }

            return _rTime_mS;
        }

        ///
        /// \brief Returns amount of time MCU has been running in milliseconds.
        /// Actual incrementation of rTime_ms must be done externally.
        /// \returns Runtime in milliseconds.
        ///
        inline uint32_t currentRunTimeUs()
        {
            uint32_t _rTime_uS;

            ATOMIC_SECTION
            {
                _rTime_uS = detail::rTime_us;
            }

            return _rTime_uS;
        }
    }    // namespace timing
}    // namespace core

#endif