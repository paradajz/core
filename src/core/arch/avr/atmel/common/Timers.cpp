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

#include <avr/interrupt.h>
#include "core/util/Util.h"
#include "core/arch/common/Timers.h"

#ifdef __AVR_ATmega16U2__

// no error here checking to reduce flash size

// 4 constant based on prescaler 64
constexpr uint32_t TIMER_US_TO_TICKS(uint32_t us)
{
    return (((us / 4) - 1));
}

namespace
{
    // use just one timer on AVR
    core::mcu::timers::handler_t _handler = nullptr;
}    // namespace

ISR(TIMER0_COMPA_vect)
{
    _handler();
}

namespace core::mcu::timers
{
    bool init()
    {
        TCCR0A = 0;
        TCCR0B = 0;
        TIMSK0 = 0;
        TCNT0  = 0;
        TCCR0A |= (1 << WGM01);                 // CTC mode
        TCCR0B |= (1 << CS01) | (1 << CS00);    // prescaler 64

        return true;
    }

    bool allocate(size_t& index, handler_t handler)
    {
        index    = 0;
        _handler = handler;

        return true;
    }

    bool start(size_t index)
    {
        TIMSK0 |= (1 << OCIE0A);    // compare match interrupt
        return true;
    }

    void startAll()
    {
        start(0);
    }

    bool stop(size_t index)
    {
        TIMSK0 &= ~(1 << OCIE0A);
        TCNT0 = 0;

        return true;
    }

    void stopAll()
    {
        stop(0);
    }

    bool isRunning(size_t index)
    {
        return core::util::BIT_READ(TIMSK0, OCIE0A);
    }

    bool setPeriod(size_t index, uint32_t us)
    {
        OCR0A = TIMER_US_TO_TICKS(us);
        return true;
    }
}    // namespace core::mcu::timers

#endif