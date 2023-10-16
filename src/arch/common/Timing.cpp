/*
    Copyright 2017-2023 Igor Petrovic

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

#include "core/MCU.h"

namespace
{
    constexpr uint32_t PERIOD_US = 1000;
    volatile uint32_t  mcuMs;
}    // namespace

namespace core::mcu::timing
{
    void init()
    {
        size_t timerIndex = 0;

        core::mcu::timers::allocate(timerIndex, []()
                                    {
                                        mcuMs++;
                                    });

        core::mcu::timers::setPeriod(timerIndex, PERIOD_US);
        core::mcu::timers::start(timerIndex);
    }

    uint32_t ms()
    {
        uint32_t temp;

        CORE_MCU_ATOMIC_SECTION
        {
            temp = mcuMs;
        }

        return temp;
    }
}    // namespace core::mcu::timing