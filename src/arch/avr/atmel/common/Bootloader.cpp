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

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <CoreMCUGenerated.h>
#include "core/src/arch/avr/common/Atomic.h"

namespace core::mcu::bootloader
{
    bool erasePage(size_t index)
    {
        CORE_MCU_ATOMIC_SECTION
        {
            boot_page_erase(index * core::mcu::flash::pageSize(index));
            boot_spm_busy_wait();
        }

        return true;
    }

    bool fillPage(size_t index, uint32_t addressInPage, uint32_t data)
    {
        CORE_MCU_ATOMIC_SECTION
        {
            boot_page_fill_safe(CORE_MCU_FLASH_PAGE_ADDR(index) + addressInPage, data & static_cast<uint16_t>(0xFFFF));
            boot_page_fill_safe(CORE_MCU_FLASH_PAGE_ADDR(index) + addressInPage + 2, data >> 16);
        }

        return true;
    }

    bool commitPage(size_t index)
    {
        CORE_MCU_ATOMIC_SECTION
        {
            // write the filled flash page to memory
            boot_page_write(index * core::mcu::flash::pageSize(index));
            boot_spm_busy_wait();

            // re-enable RWW section
            boot_rww_enable();
        }

        return true;
    }
}    // namespace core::mcu::bootloader