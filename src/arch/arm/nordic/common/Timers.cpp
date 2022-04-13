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

#include <vector>
#include "app_timer.h"
#include "core/src/arch/common/Timers.h"
#include "core/src/ErrorHandler.h"

namespace
{
    constexpr uint32_t MIN_TIMEOUT_US = 200;

#if APP_TIMER_CONFIG_RTC_FREQUENCY == 0
    constexpr uint32_t US_TIMER_TICKS(uint32_t us)
    {
        return (7 * (us / MIN_TIMEOUT_US));
    }
#else
#error US_TIMER_TICKS function doesn't work if APP_TIMER_CONFIG_RTC_FREQUENCY isn't set to 0
#endif

    struct appTimer_t
    {
        app_timer_t                  instance   = {};
        core::mcu::timers::handler_t handler    = nullptr;
        uint32_t                     intervalUs = 0;
        bool                         started    = false;
    };

    std::vector<appTimer_t> _timer;
}    // namespace

extern "C" void appTimerHandler(void* context)
{
    auto appTimerInstance = reinterpret_cast<appTimer_t*>(context);
    appTimerInstance->handler();
}

namespace core::mcu::timers
{
    bool init()
    {
        CORE_ERROR_CHECK(app_timer_init(), NRF_SUCCESS);
        return true;
    }

    bool allocate(size_t& index, handler_t handler)
    {
        appTimer_t timer;
        timer.handler = handler;
        index         = _timer.size();
        _timer.push_back(timer);

        const app_timer_id_t PTR = &_timer.at(index).instance;

        CORE_ERROR_CHECK(app_timer_create(&PTR, APP_TIMER_MODE_REPEATED, appTimerHandler), NRF_SUCCESS);

        return true;
    }

    bool start(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer.at(index).intervalUs)
        {
            return false;
        }

        stop(index);
        CORE_ERROR_CHECK(app_timer_start(&_timer.at(index).instance,
                                         US_TIMER_TICKS(_timer.at(index).intervalUs),
                                         &_timer.at(index)),
                         NRF_SUCCESS);

        _timer.at(index).started = true;

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

        if (!_timer.at(index).started)
        {
            return false;
        }

        CORE_ERROR_CHECK(app_timer_stop(&_timer.at(index).instance), NRF_SUCCESS);

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

        return _timer.at(index).started;
    }

    bool setPeriod(size_t index, uint32_t us)
    {
        if (us < MIN_TIMEOUT_US)
        {
            return false;
        }

        if (index >= _timer.size())
        {
            return false;
        }

        _timer.at(index).intervalUs = us;

        return true;
    }
}    // namespace core::mcu::timers