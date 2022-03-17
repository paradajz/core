/*
    Copyright 2017-2021 Igor Petrovic

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

#ifndef __CORE_ARM_NORDIC_IO
#define __CORE_ARM_NORDIC_IO

#include <inttypes.h>
#include "../../../general/Helpers.h"
#include "nrf_gpio.h"

///
/// \brief I/O macros and functions.
/// These macros are used only to wrap NRF52 HAL API.
/// @{

namespace core
{
    namespace io
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

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        struct mcuPin_t
        {
            pinPort_t   port      = 0;
            pinIndex_t  index     = 0;
            pinMode_t   mode      = pinMode_t::INPUT;
            pullMode_t  pull      = pullMode_t::NONE;
            uint32_t    strength  = NRF_GPIO_PIN_S0S1;
            interrupt_t interrupt = interrupt_t::NONE;
        };
    }    // namespace io
}    // namespace core

#define PORT_TO_MEM(port)                   (reinterpret_cast<NRF_GPIO_Type*>(port))
#define CORE_IO_SET_LOW(port, index)        (PORT_TO_MEM(port)->OUTCLR = 1UL << index)
#define CORE_IO_SET_HIGH(port, index)       (PORT_TO_MEM(port)->OUTSET = 1UL << index)
#define CORE_IO_SET_PORT_STATE(port, state) (PORT_TO_MEM(port)->OUT = state)
#define CORE_IO_READ_OUT_PORT(port)         (PORT_TO_MEM(port)->OUT)
#define CORE_IO_READ_IN_PORT(port)          (PORT_TO_MEM(port)->IN)
#define CORE_IO_READ(port, index)           (CORE_IO_READ_PORT(port) >> index & 0x01)
#define CORE_IO_SET_STATE(port, index, state) \
    do                                        \
    {                                         \
        if (state)                            \
        {                                     \
            CORE_IO_SET_HIGH(port, index);    \
        }                                     \
        else                                  \
        {                                     \
            CORE_IO_SET_LOW(port, index);     \
        }                                     \
    } while (0)

///
/// \brief Convenience macros for portable GPIO port/pin definitions across various toolchains.
/// @{
#define CORE_IO_PIN_PORT_DEF(port)   (NRF_P##port##_BASE)
#define CORE_IO_PIN_INDEX_DEF(index) (index)

/// @}

///
/// \brief Convenience macro used to create mcuPin_t structure.
///
#define CORE_IO_MCU_PIN_VAR(_port, _index) \
    {                                      \
        .port  = _port,                    \
        .index = _index                    \
    }

///
/// \brief Macros used to retrieve either pin port or pin index from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_PORT(mcuPin)  mcuPin.port
#define CORE_IO_MCU_PIN_INDEX(mcuPin) mcuPin.index

/// @}

inline void CORE_IO_DEINIT(core::io::mcuPin_t pin)
{
    PORT_TO_MEM(pin.port)->PIN_CNF[pin.index] = (NRF_GPIO_PIN_DIR_INPUT << GPIO_PIN_CNF_DIR_Pos) |
                                                (NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos) |
                                                (static_cast<uint32_t>(core::io::pullMode_t::NONE) << GPIO_PIN_CNF_PULL_Pos) |
                                                (NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                                (static_cast<uint32_t>(core::io::interrupt_t::NONE) << GPIO_PIN_CNF_SENSE_Pos);
}

inline void CORE_IO_INIT(core::io::mcuPin_t pin)
{
    uint32_t direction    = NRF_GPIO_PIN_DIR_INPUT;
    uint32_t inputConnect = NRF_GPIO_PIN_INPUT_CONNECT;

    switch (pin.mode)
    {
    case core::io::pinMode_t::INPUT:
    {
        // nothing to do
    }
    break;

    case core::io::pinMode_t::OUTPUT_PP:
    {
        direction    = NRF_GPIO_PIN_DIR_OUTPUT;
        inputConnect = NRF_GPIO_PIN_INPUT_DISCONNECT;
    }
    break;

    case core::io::pinMode_t::OUTPUT_OD:
    {
        direction    = NRF_GPIO_PIN_DIR_OUTPUT;
        inputConnect = NRF_GPIO_PIN_INPUT_DISCONNECT;
        pin.strength = GPIO_PIN_CNF_DRIVE_S0D1;
    }
    break;

    case core::io::pinMode_t::ANALOG:
    default:
    {
        CORE_IO_DEINIT(pin);
        return;
    }
    break;
    }

    PORT_TO_MEM(pin.port)->PIN_CNF[pin.index] = (direction << GPIO_PIN_CNF_DIR_Pos) |
                                                (inputConnect << GPIO_PIN_CNF_INPUT_Pos) |
                                                (static_cast<uint32_t>(pin.pull) << GPIO_PIN_CNF_PULL_Pos) |
                                                (pin.strength << GPIO_PIN_CNF_DRIVE_Pos) |
                                                (static_cast<uint32_t>(pin.interrupt) << GPIO_PIN_CNF_SENSE_Pos);
}

/// @}

#endif