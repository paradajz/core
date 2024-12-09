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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

#include "nrfx_clock.h"

namespace core::mcu::clocks
{
    void init()
    {
        nrfx_clock_init([](nrfx_clock_evt_type_t event)
                        {
                            switch (event)
                            {
                            case NRFX_CLOCK_EVT_HFCLK_STARTED:
                            {
                            }
                            break;

                            case NRFX_CLOCK_EVT_LFCLK_STARTED:
                            {
                            }
                            break;

                            default:
                                break;
                            }
                        });

        nrfx_clock_hfclk_start();
        nrfx_clock_lfclk_start();

        while (!nrfx_clock_hfclk_is_running())
        {
        }

        while (!nrfx_clock_lfclk_is_running())
        {
        }
    }
}    // namespace core::mcu::clocks

#pragma GCC diagnostic pop