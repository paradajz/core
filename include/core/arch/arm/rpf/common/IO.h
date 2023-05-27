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
#include "hardware/gpio.h"

namespace core::mcu::io
{
    enum class pinMode_t : uint32_t
    {
        INPUT,        // Input Floating Mode
        OUTPUT_PP,    // Output Push Pull Mode
        ANALOG        // Just de-init the pin with this option
    };

    enum class interrupt_t : uint32_t
    {
        // match with enum in pico sdk
        // these are masks - multiple can be set at the same time
        LOW,
        HIGH,
        FALLING,
        RISING,
        NONE,
    };

    using pinPort_t   = uint32_t;
    using pinIndex_t  = uint32_t;
    using portWidth_t = uint32_t;

    enum class pullMode_t : uint32_t
    {
        NONE,
        DOWN,
        UP
    };

    /// Structure used to define single MCU pin.
    struct pin_t
    {
        pinPort_t           port      = 0;
        pinIndex_t          index     = 0;
        pinMode_t           mode      = pinMode_t::INPUT;
        pullMode_t          pull      = pullMode_t::NONE;
        gpio_drive_strength strength  = GPIO_DRIVE_STRENGTH_2MA;
        uint32_t            interrupt = static_cast<uint32_t>(interrupt_t::NONE);

        constexpr pin_t(pinPort_t  port,
                        pinIndex_t index)
            : port(port)
            , index(index)
        {}

        constexpr pin_t(pinPort_t  port,
                        pinIndex_t index,
                        pinMode_t  mode,
                        pullMode_t pull)
            : port(port)
            , index(index)
            , mode(mode)
            , pull(pull)
        {}

        constexpr pin_t(pinPort_t           port,
                        pinIndex_t          index,
                        pinMode_t           mode,
                        pullMode_t          pull,
                        gpio_drive_strength strength,
                        uint32_t            interrupt)
            : port(port)
            , index(index)
            , mode(mode)
            , pull(pull)
            , strength(strength)
            , interrupt(interrupt)
        {}

        pin_t() = default;
    };
}    // namespace core::mcu::io

#define CORE_MCU_IO_PORT_TO_MEM(port)
#define CORE_MCU_IO_SET_LOW(port, index)          gpio_put(index, false)
#define CORE_MCU_IO_SET_HIGH(port, index)         gpio_put(index, true)
#define CORE_MCU_IO_SET_PORT_STATE(port, state)   gpio_put_all(state)
#define CORE_MCU_IO_READ_IN_PORT(port)            gpio_get_all()
#define CORE_MCU_IO_READ_OUT_PORT(port)           CORE_MCU_IO_READ_IN_PORT(port)
#define CORE_MCU_IO_READ(port, index)             gpio_get(index)
#define CORE_MCU_IO_SET_STATE(port, index, state) gpio_put(index, state)

/// Convenience macros for portable GPIO port/pin definitions across various toolchains.
#define CORE_MCU_IO_PIN_PORT_DEF(port)   0
#define CORE_MCU_IO_PIN_INDEX_DEF(index) (index)

inline void CORE_MCU_IO_DEINIT(core::mcu::io::pin_t pin)
{
    gpio_deinit(pin.index);
}

inline void CORE_MCU_IO_INIT(core::mcu::io::pin_t pin)
{
    gpio_init(pin.index);
    gpio_set_dir(pin.index, pin.mode == core::mcu::io::pinMode_t::INPUT ? GPIO_IN : GPIO_OUT);
    gpio_set_pulls(pin.index, pin.pull == core::mcu::io::pullMode_t::UP, pin.pull == core::mcu::io::pullMode_t::DOWN);
    gpio_set_drive_strength(pin.index, pin.strength);

    if (pin.interrupt != static_cast<uint32_t>(core::mcu::io::interrupt_t::NONE))
    {
        gpio_set_irq_enabled(pin.index, pin.interrupt, true);
    }
}

inline void CORE_MCU_IO_INIT(core::mcu::io::pinPort_t  port,
                             core::mcu::io::pinIndex_t index,
                             core::mcu::io::pinMode_t  mode = core::mcu::io::pinMode_t::INPUT,
                             core::mcu::io::pullMode_t pull = core::mcu::io::pullMode_t::NONE)
{
    core::mcu::io::pin_t pin;

    pin.port  = port;
    pin.index = index;
    pin.mode  = mode;
    pin.pull  = pull;

    CORE_MCU_IO_INIT(pin);
}

inline void CORE_MCU_IO_DEINIT(core::mcu::io::pinPort_t  port,
                               core::mcu::io::pinIndex_t index)
{
    gpio_deinit(index);
}
