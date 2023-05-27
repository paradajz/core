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

#pragma once

#include <inttypes.h>

#ifdef CORE_MCU_STUB
#include "arch/stub/Timing.h"
#else
#ifdef CORE_MCU_ARCH_AVR
#include "arch/avr/atmel/common/Timing.h"
#elif defined(CORE_MCU_ARCH_ARM)
#ifdef CORE_MCU_VENDOR_ST
#include "arch/arm/st/common/Timing.h"
#elif defined(CORE_MCU_VENDOR_NORDIC)
#include "arch/arm/nordic/common/Timing.h"
#elif defined(CORE_MCU_VENDOR_RPF)
#include "arch/arm/rpf/common/Timing.h"
#else
#include "arch/stub/Timing.h"
#endif
#else
#include "arch/stub/Timing.h"
#endif
#endif

#include "MCU.h"

namespace core::timing
{
    namespace detail
    {
        /// Definition of variable holding current MCU run time in milliseconds.
        /// Must be implemented externally in order to use core::timing::ms() function.
        extern volatile uint32_t ms;

        /// Definition of variable holding current MCU run time in microseconds.
        /// Must be implemented externally in order to use core::timing::us() function.
        extern volatile uint32_t us;
    }    // namespace detail

    inline uint32_t ms()
    {
        uint32_t temp;

        CORE_MCU_ATOMIC_SECTION
        {
            temp = detail::ms;
        }

        return temp;
    }

    inline uint32_t us()
    {
        uint32_t temp;

        CORE_MCU_ATOMIC_SECTION
        {
            temp = detail::us;
        }

        return temp;
    }
}    // namespace core::timing
