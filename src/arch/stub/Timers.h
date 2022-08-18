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
#include <cstddef>
#ifndef CORE_USE_C_TIMER_CALLBACK
#include <functional>
#endif

namespace core::mcu::timers
{
#ifdef CORE_USE_C_TIMER_CALLBACK
    using handler_t = void (*)();
#else
    using handler_t = std::function<void()>;
#endif

    inline bool init()
    {
        return false;
    }

    inline bool allocate(size_t& index, handler_t handler)
    {
        return false;
    }

    inline bool setPeriod(size_t index, uint32_t us)
    {
        return false;
    }

    inline bool start(size_t index)
    {
        return false;
    }

    inline void startAll()
    {
    }

    inline bool stop(size_t index)
    {
        return false;
    }

    inline void stopAll()
    {
    }

    inline bool isRunning(size_t index)
    {
        return false;
    }
}    // namespace core::mcu::timers
