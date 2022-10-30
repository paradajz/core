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

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include <array>
#include "core/src/arch/arm/common/Atomic.h"
#include "HAL.h"
#include "IO.h"
#include "Peripherals.h"
#include "core/src/arch/common/ADC.h"
#include "core/src/arch/common/Bootloader.h"
#include "core/src/arch/common/I2C.h"
#include "core/src/arch/common/Clocks.h"
#include "core/src/arch/common/Flash.h"
#include "core/src/arch/common/ISR.h"
#include "core/src/arch/common/Timers.h"
#include "core/src/arch/common/UART.h"
#include "core/src/ErrorHandler.h"
#include <MCU.h>
#include "core/src/arch/common/usb/USB.h"
#include "core/src/arch/common/MCU.h"

namespace core::mcu
{
    inline void init(initType_t initType = initType_t::APP)
    {
        HAL_Init();

        clocks::init();
        flash::init();
    }

    inline void deInit()
    {
        HAL_RCC_DeInit();
        HAL_DeInit();
    }

    inline void reset()
    {
        NVIC_SystemReset();
    }

    inline void uniqueID(uniqueID_t& uid)
    {
        uint32_t id[3];

        id[0] = HAL_GetUIDw0();
        id[1] = HAL_GetUIDw1();
        id[2] = HAL_GetUIDw2();

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                uid[(i * 4) + j] = id[i] >> ((3 - j) * 8) & 0xFF;
            }
        }
    }

    inline void idle()
    {
    }
}    // namespace core::mcu