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
#include <avr/interrupt.h>
#include "core/util/Util.h"
#include "core/arch/common/Timers.h"

namespace
{
    // 4 constant based on prescaler 64
    constexpr uint32_t TIMER_US_TO_TICKS(uint32_t us)
    {
        return (((us / 4) - 1));
    }

    constexpr size_t NUM_OF_TIMERS = 2;

    struct avrTimer_t
    {
        bool                         allocated = false;
        core::mcu::timers::handler_t handler   = nullptr;
    };

    std::array<avrTimer_t, NUM_OF_TIMERS> _timer;
}    // namespace

ISR(TIMER0_COMPA_vect)
{
    if (_timer[0].handler != nullptr)
    {
        _timer[0].handler();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (_timer[1].handler != nullptr)
    {
        _timer[1].handler();
    }
}

namespace core::mcu::timers
{
    bool init()
    {
        // timer0
        TCCR0A = 0;
        TCCR0B = 0;
        TIMSK0 = 0;
        TCNT0  = 0;
        TCCR0A |= (1 << WGM01);                 // CTC mode
        TCCR0B |= (1 << CS01) | (1 << CS00);    // prescaler 64

        // timer1
        TCCR1A = 0;
        TCCR1B = 0;
        TCCR1C = 0;
        TIMSK1 = 0;
        TCNT1  = 0;
        TCCR1B |= (1 << WGM12);                 // CTC mode
        TCCR1B |= (1 << CS11) | (1 << CS10);    // prescaler 64

        return true;
    }

    bool allocate(size_t& index, handler_t handler)
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
        {
            if (!_timer[i].allocated)
            {
                _timer[i].allocated = true;
                index               = i;
                _timer[i].handler   = handler;

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

        // enable compare match interrupt
        switch (index)
        {
        case 0:
        {
            TIMSK0 |= (1 << OCIE0A);
        }
        break;

        case 1:
        {
            TIMSK1 |= (1 << OCIE1A);
        }
        break;

        default:
            return false;
        }

        return true;
    }

    void startAll()
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
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

        if (!isRunning(index))
        {
            return false;
        }

        // disable compare match interrupt and clear counter
        switch (index)
        {
        case 0:
        {
            TIMSK0 &= ~(1 << OCIE0A);
            TCNT0 = 0;
        }
        break;

        case 1:
        {
            TIMSK1 &= ~(1 << OCIE1A);
            TCNT1 = 0;
        }
        break;

        default:
            return false;
        }

        return true;
    }

    void stopAll()
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
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

        return core::util::BIT_READ(TIMSK0, OCIE0A);
    }

    bool setPeriod(size_t index, uint32_t us)
    {
        if (!_timer[index].allocated)
        {
            return false;
        }

        switch (index)
        {
        case 0:
        {
            OCR0A = TIMER_US_TO_TICKS(us);
        }
        break;

        case 1:
        {
            OCR1A = TIMER_US_TO_TICKS(us);
        }
        break;

        default:
            return false;
        }

        return true;
    }
}    // namespace core::mcu::timers