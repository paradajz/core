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

#include <array>
#include "core/arch/arm/common/atomic.h"
#include "hal.h"
#include "peripherals.h"
#include "core/arch/common/adc.h"
#include "core/arch/common/bootloader.h"
#include "core/arch/common/i2c.h"
#include "core/arch/common/clocks.h"
#include "core/arch/common/flash.h"
#include "core/arch/common/isr.h"
#include "core/arch/common/timers.h"
#include "core/arch/common/timing.h"
#include "core/arch/common/uart.h"
#include "core/error_handler.h"
#include "core/arch/common/usb/usb.h"
#include "core/arch/common/mcu.h"

namespace core::mcu
{
    inline void init(initType_t initType = initType_t::APP)
    {
        // always set voltage mode to 3V (if not already set)
        if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) != (UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos))
        {
            NRF_NVMC->CONFIG = NRF_NVMC_MODE_WRITE;

            while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
            {
            }

            NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                                (UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos);

            NRF_NVMC->CONFIG = NRF_NVMC_MODE_READONLY;

            while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
            {
            }

            // system reset is needed to update UICR registers
            NVIC_SystemReset();
        }

        sd_softdevice_vector_table_base_set(reinterpret_cast<uint32_t>(__flash_start__));

        clocks::init();
        flash::init();
        timers::init();
        timing::init();
    }

    inline void deInit()
    {
        core::mcu::timers::stopAll();
    }

    inline void reset()
    {
        NVIC_SystemReset();
    }

    inline void uniqueID(uniqueID_t& uid)
    {
        uint32_t id[2];

        id[0] = NRF_FICR->DEVICEADDR[0];
        id[1] = NRF_FICR->DEVICEADDR[1];

        for (int i = 0; i < 2; i++)
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