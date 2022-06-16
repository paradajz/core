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
#include "core/src/util/Util.h"
#include "nrf_gpio.h"

namespace core::mcu::io
{
    enum class pinMode_t : uint32_t
    {
        INPUT,        // Input Floating Mode
        OUTPUT_PP,    // Output Push Pull Mode
        OUTPUT_OD,    // Output Open Drain Mode
        ANALOG        // Just de-init the pin with this option
    };

    enum class interrupt_t : uint32_t
    {
        NONE = NRF_GPIO_PIN_NOSENSE,
        LOW  = NRF_GPIO_PIN_SENSE_LOW,
        HIGH = NRF_GPIO_PIN_SENSE_HIGH
    };

    using pinPort_t   = uint32_t;
    using pinIndex_t  = uint8_t;
    using portWidth_t = uint32_t;

    enum class pullMode_t : uint32_t
    {
        NONE = 0x00000000U,
        DOWN = 0x00000001U,
        UP   = 0x00000003U,
    };

    /// Structure used to define single MCU pin.
    struct pin_t
    {
        pinPort_t   port      = 0;
        pinIndex_t  index     = 0;
        pinMode_t   mode      = pinMode_t::INPUT;
        pullMode_t  pull      = pullMode_t::NONE;
        uint32_t    strength  = NRF_GPIO_PIN_S0S1;
        interrupt_t interrupt = interrupt_t::NONE;

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

        constexpr pin_t(pinPort_t   port,
                        pinIndex_t  index,
                        pinMode_t   mode,
                        pullMode_t  pull,
                        uint32_t    strength,
                        interrupt_t interrupt)
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

#define CORE_MCU_IO_PORT_TO_MEM(port)           (reinterpret_cast<NRF_GPIO_Type*>(port))
#define CORE_MCU_IO_SET_LOW(port, index)        (CORE_MCU_IO_PORT_TO_MEM(port)->OUTCLR = 1UL << index)
#define CORE_MCU_IO_SET_HIGH(port, index)       (CORE_MCU_IO_PORT_TO_MEM(port)->OUTSET = 1UL << index)
#define CORE_MCU_IO_SET_PORT_STATE(port, state) (CORE_MCU_IO_PORT_TO_MEM(port)->OUT = state)
#define CORE_MCU_IO_READ_OUT_PORT(port)         (CORE_MCU_IO_PORT_TO_MEM(port)->OUT)
#define CORE_MCU_IO_READ_IN_PORT(port)          (CORE_MCU_IO_PORT_TO_MEM(port)->IN)
#define CORE_MCU_IO_READ(port, index)           (CORE_MCU_IO_READ_IN_PORT(port) >> index & 0x01)
#define CORE_MCU_IO_SET_STATE(port, index, state) \
    do                                            \
    {                                             \
        if (state)                                \
        {                                         \
            CORE_MCU_IO_SET_HIGH(port, index);    \
        }                                         \
        else                                      \
        {                                         \
            CORE_MCU_IO_SET_LOW(port, index);     \
        }                                         \
    } while (0)

/// Convenience macros for portable GPIO port/pin definitions across various toolchains.
#define CORE_MCU_IO_PIN_PORT_DEF(port)   (NRF_P##port##_BASE)
#define CORE_MCU_IO_PIN_INDEX_DEF(index) (index)

// modification of NRF_GPIO_PIN_MAP macro which works with raw port indexes (0 or 1)
// this function works with CORE_MCU_IO_PIN_PORT_DEF port definitions instead
inline uint32_t CORE_NRF_GPIO_PIN_MAP(core::mcu::io::pinPort_t port, core::mcu::io::pinIndex_t index)
{
    switch (port)
    {
    case NRF_P0_BASE:
        return NRF_GPIO_PIN_MAP(0, index);

    case NRF_P1_BASE:
        return NRF_GPIO_PIN_MAP(1, index);

    default:
        return 0xFFFFFFFF;
    }
}

inline void CORE_MCU_IO_DEINIT(core::mcu::io::pin_t pin)
{
    CORE_MCU_IO_PORT_TO_MEM(pin.port)->PIN_CNF[pin.index] = (NRF_GPIO_PIN_DIR_INPUT << GPIO_PIN_CNF_DIR_Pos) |
                                                            (NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos) |
                                                            (static_cast<uint32_t>(core::mcu::io::pullMode_t::NONE) << GPIO_PIN_CNF_PULL_Pos) |
                                                            (NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                                            (static_cast<uint32_t>(core::mcu::io::interrupt_t::NONE) << GPIO_PIN_CNF_SENSE_Pos);
}

inline void CORE_MCU_IO_INIT(core::mcu::io::pin_t pin)
{
    uint32_t direction    = NRF_GPIO_PIN_DIR_INPUT;
    uint32_t inputConnect = NRF_GPIO_PIN_INPUT_CONNECT;

    switch (pin.mode)
    {
    case core::mcu::io::pinMode_t::INPUT:
    {
        // nothing to do
    }
    break;

    case core::mcu::io::pinMode_t::OUTPUT_PP:
    {
        direction    = NRF_GPIO_PIN_DIR_OUTPUT;
        inputConnect = NRF_GPIO_PIN_INPUT_DISCONNECT;
    }
    break;

    case core::mcu::io::pinMode_t::OUTPUT_OD:
    {
        direction    = NRF_GPIO_PIN_DIR_OUTPUT;
        inputConnect = NRF_GPIO_PIN_INPUT_DISCONNECT;
        pin.strength = GPIO_PIN_CNF_DRIVE_S0D1;
    }
    break;

    case core::mcu::io::pinMode_t::ANALOG:
    default:
    {
        CORE_MCU_IO_DEINIT(pin);
        return;
    }
    break;
    }

    CORE_MCU_IO_PORT_TO_MEM(pin.port)->PIN_CNF[pin.index] = (direction << GPIO_PIN_CNF_DIR_Pos) |
                                                            (inputConnect << GPIO_PIN_CNF_INPUT_Pos) |
                                                            (static_cast<uint32_t>(pin.pull) << GPIO_PIN_CNF_PULL_Pos) |
                                                            (pin.strength << GPIO_PIN_CNF_DRIVE_Pos) |
                                                            (static_cast<uint32_t>(pin.interrupt) << GPIO_PIN_CNF_SENSE_Pos);
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
    core::mcu::io::pin_t pin;

    pin.port  = port;
    pin.index = index;

    CORE_MCU_IO_DEINIT(pin);
}
