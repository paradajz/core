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
#include "app_timer.h"
#include "nrfx_timer.h"
#include "core/arch/common/timers.h"
#include "core/error_handler.h"

namespace
{
    // 5 timers are available, but timer 0 is reserved for soft device
    constexpr size_t NUM_OF_TIMERS = 4;

    struct timer_t
    {
        nrfx_timer_t                 instance  = {};
        core::mcu::timers::handler_t handler   = nullptr;
        bool                         started   = false;
        uint32_t                     context   = 0;
        bool                         allocated = false;
    };

    std::array<timer_t, NUM_OF_TIMERS> _timer;
}    // namespace

extern "C" void timerHandler(nrf_timer_event_t event, void* context)
{
    switch (event)
    {
    case NRF_TIMER_EVENT_COMPARE0:
    {
        auto appTimerInstance = *reinterpret_cast<uint32_t*>(context);
        _timer[appTimerInstance].handler();
    }
    break;

    default:
        break;
    }
}

namespace core::mcu::timers
{
    bool init()
    {
        // needed for softdevice
        CORE_ERROR_CHECK(app_timer_init(), NRF_SUCCESS);

        _timer[0].instance = NRFX_TIMER_INSTANCE(1);
        _timer[1].instance = NRFX_TIMER_INSTANCE(2);
        _timer[2].instance = NRFX_TIMER_INSTANCE(3);
        _timer[3].instance = NRFX_TIMER_INSTANCE(4);

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
                _timer[i].context   = i;
                index               = i;

                nrfx_timer_config_t config = {
                    .frequency          = NRF_TIMER_FREQ_16MHz,
                    .mode               = NRF_TIMER_MODE_TIMER,
                    .bit_width          = NRF_TIMER_BIT_WIDTH_32,
                    .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
                    .p_context          = &_timer[i].context
                };

                CORE_ERROR_CHECK(nrfx_timer_init(&_timer[i].instance,
                                                 &config,
                                                 timerHandler),
                                 NRFX_SUCCESS);

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

        stop(index);

        nrfx_timer_enable(&_timer[index].instance);
        _timer[index].started = true;

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

        if (!_timer[index].started)
        {
            return false;
        }

        nrfx_timer_disable(&_timer[index].instance);
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

        auto     prescaler = static_cast<uint32_t>(nrf_timer_frequency_get(_timer[index].instance.p_reg));
        uint64_t ticks     = ((us * 16ULL) >> prescaler);

        NRFX_ASSERT(ticks <= UINT32_MAX);

        nrfx_timer_extended_compare(&_timer[index].instance,
                                    NRF_TIMER_CC_CHANNEL0,
                                    ticks,
                                    NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                    true);

        return true;
    }
}    // namespace core::mcu::timers