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

#include "core/src/arch/common/ADC.h"
#include "core/src/arch/common/Clocks.h"
#include "core/src/arch/common/Flash.h"
#include "core/src/arch/common/ISR.h"
#include "core/src/arch/common/Timers.h"
#include "ADC.h"
#include "Atomic.h"
#include "I2C.h"
#include "Interrupt.h"
#include "IO.h"
#include "Peripherals.h"
#include "UART.h"
#include "Util.h"
#if __has_include(<MCU.h>)
#include <MCU.h>
#else
// already defined in generated MCU, define only if it doesn't exist
constexpr inline size_t core::mcu::UID_BITS = 80;
#endif
#include "core/src/arch/common/MCU.h"

namespace core::mcu
{
    inline void init(initType_t initType = initType_t::APP)
    {
    }

    inline void deInit()
    {
    }

    inline void reset()
    {
    }

    inline void uniqueID(uniqueID_t& uid)
    {
    }

    inline void idle()
    {
    }
}    // namespace core::mcu