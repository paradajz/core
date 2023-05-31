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

#ifndef CORE_MCU_STUB
#define CORE_MCU_STUB
#endif

#ifndef CORE_MCU_UID_BITS
#define CORE_MCU_UID_BITS 80
#endif

#define CORE_MCU_NOP()

#include <inttypes.h>
#include "ADC.h"
#include "Atomic.h"
#include "Bootloader.h"
#include "Clocks.h"
#include "Flash.h"
#include "I2C.h"
#include "Interrupt.h"
#include "IO.h"
#include "ISR.h"
#include "Peripherals.h"
#include "Timers.h"
#include "UART.h"
#include "USB.h"
#include "Util.h"
#include "core/arch/common/usb/USB.h"

namespace core::mcu
{
    using uniqueID_t = std::array<uint8_t, CORE_MCU_UID_BITS / 8>;

    enum class initType_t : uint8_t
    {
        APP,
        BOOT
    };

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