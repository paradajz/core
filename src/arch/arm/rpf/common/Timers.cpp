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

#include <array>
#include "core/arch/common/Timers.h"
#include "core/ErrorHandler.h"
#include "pico/types.h"
#include "pico/time.h"

namespace
{
    // hardware limit
    constexpr size_t NUM_OF_TIMERS = 4;

    struct timer_t
    {
        core::mcu::timers::handler_t handler    = nullptr;
        uint32_t                     intervalUs = 0;
        bool                         started    = false;
        bool                         allocated  = false;
    };

    std::array<timer_t, NUM_OF_TIMERS> _timer;
}    // namespace

extern "C" void timerHandler(uint id)
{
    if (_timer[id].handler != nullptr)
    {
        _timer[id].handler();
        auto timeTarget = make_timeout_time_us(_timer[id].intervalUs);
        hardware_alarm_set_target(id, timeTarget);
    }
}

namespace core::mcu::timers
{
    bool init()
    {
        return true;
    }

    bool allocate(size_t& index, handler_t handler)
    {
        for (size_t i = 0; i < _timer.size(); i++)
        {
            if (!_timer[i].allocated)
            {
                _timer[i].allocated = true;
                _timer[i].handler   = handler;
                index               = i;

                hardware_alarm_claim(index);
                hardware_alarm_set_callback(index, timerHandler);

                return true;
            }
        }

        return false;
    }

    bool start(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        if (!_timer[index].intervalUs)
        {
            return false;
        }

        stop(index);
        _timer[index].started = true;
        hardware_alarm_set_target(index, make_timeout_time_us(_timer[index].intervalUs));

        return true;
    }

    void startAll()
    {
        for (size_t i = 0; i < _timer.size(); i++)
        {
            start(i);
        }
    }

    bool stop(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        if (!_timer[index].started)
        {
            return false;
        }

        hardware_alarm_cancel(index);
        _timer[index].started = false;

        return true;
    }

    void stopAll()
    {
        for (size_t i = 0; i < _timer.size(); i++)
        {
            stop(i);
        }
    }

    bool isRunning(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        return _timer[index].started;
    }

    bool setPeriod(size_t index, uint32_t us)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        _timer[index].intervalUs = us;

        return true;
    }
}    // namespace core::mcu::timers