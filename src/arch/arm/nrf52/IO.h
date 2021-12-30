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

#ifndef __CORE_ARM_NRF52_IO
#define __CORE_ARM_NRF52_IO

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
            input,       // Input Floating Mode
            outputPP,    // Output Push Pull Mode
            outputOD,    // Output Open Drain Mode
            outputCS     // Output custom drive strength
        };

        enum class interrupt_t : uint32_t
        {
            none = NRF_GPIO_PIN_NOSENSE,
            low  = NRF_GPIO_PIN_SENSE_LOW,
            high = NRF_GPIO_PIN_SENSE_HIGH
        };

        using gpioPinPort_t  = NRF_GPIO_Type*;
        using gpioPinIndex_t = uint16_t;

        enum class pullMode_t : uint32_t
        {
            none = 0x00000000U,
            down = 0x00000001U,
            up   = 0x00000003U,
        };

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        typedef struct
        {
            gpioPinPort_t  port      = NULL;
            gpioPinIndex_t index     = 0;
            pinMode_t      mode      = pinMode_t::input;
            pullMode_t     pull      = pullMode_t::none;
            uint32_t       strength  = NRF_GPIO_PIN_S0S1;
            interrupt_t    interrupt = interrupt_t::none;
        } mcuPin_t;
    }    // namespace io
}    // namespace core

inline void CORE_IO_CONFIG(core::io::mcuPin_t pin)
{
    uint32_t strength;
    uint32_t direction    = NRF_GPIO_PIN_DIR_INPUT;
    uint32_t inputConnect = NRF_GPIO_PIN_INPUT_CONNECT;

    switch (pin.mode)
    {
    case pinMode_t::input:
    {
        // Standard '0', standard '1'
        strength = NRF_GPIO_PIN_S0S1;
    }
    break;

    case pinMode_t::outputPP:
    {
        // Standard '0', high-drive '1'
        strength = GPIO_PIN_CNF_DRIVE_S0H1;
    }
    break;

    case pinMode_t::outputOD:
    {
        strength = GPIO_PIN_CNF_DRIVE_S0D1;
    }
    break;

    case pinMode_t::outputCS:
    default:
    {
        // Standard '0', disconnect '1'
        strength     = pin.strength;
        direction    = NRF_GPIO_PIN_DIR_OUTPUT;
        inputConnect = NRF_GPIO_PIN_INPUT_DISCONNECT;
    }
    break;
    }

    pin.port->PIN_CNF[pin.index] = (direction << GPIO_PIN_CNF_DIR_Pos) |
                                   (inputConnect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (static_cast<uint32_t>(pin.pull) << GPIO_PIN_CNF_PULL_Pos) |
                                   (strength << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (static_cast<uint32_t>(pin.interrupt) << GPIO_PIN_CNF_SENSE_Pos);
}

///
/// \brief Convenience macros for portable GPIO port/pin definitions across various toolchains.
/// @{

#define CORE_IO_PORT(port)        NRF_P##port
#define CORE_IO_PORT_INDEX(index) index

/// @}

#define CORE_IO_SET_LOW(port, index)  (port->OUTCLR = 1UL << index)
#define CORE_IO_SET_HIGH(port, index) (port->OUTSET = 1UL << index)
#define CORE_IO_SET_STATE(port, index, state) \
    do                                        \
    {                                         \
        if (state)                            \
            CORE_IO_SET_HIGH(port, index);    \
        else                                  \
            CORE_IO_SET_LOW(port, index);     \
    } while (0)

#define CORE_IO_READ(port, index) (port->IN & index)

///
/// \brief Convenience macro to easily create mcuPin_t instances.
///
#define CORE_IO_MCU_PIN_DEF(_port, _index) \
    {                                      \
        .port = _port, .index = _index     \
    }

///
/// \brief Macros used to retrieve either port or pin from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_PORT(mcuPin)  mcuPin.port
#define CORE_IO_MCU_PIN_INDEX(mcuPin) mcuPin.index

/// @}

/// @}

#endif