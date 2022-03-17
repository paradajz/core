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

#ifndef __CORE_GENERAL_TIMING
#define __CORE_GENERAL_TIMING

#include <inttypes.h>
#ifdef CORE_ARCH_AVR
#include "../arch/avr/Timing.h"
#elif defined(CORE_ARCH_ARM)
#ifdef CORE_VENDOR_ST
#include "../arch/arm/st/Timing.h"
#elif defined CORE_VENDOR_NORDIC
#include "../arch/arm/nordic/Timing.h"
#endif
#else
#include "../arch/stub/Timing.h"
#endif
#include "Atomic.h"

namespace core::timing
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
}    // namespace core::timing

#endif