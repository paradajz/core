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

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <inttypes.h>

namespace core::mcu::io
{
    using pinPort_t   = uint16_t;
    using pinIndex_t  = uint8_t;
    using portWidth_t = pinIndex_t;

    enum class pinMode_t : uint8_t
    {
        INPUT,
        OUTPUT,
        ANALOG    = INPUT,
        OUTPUT_PP = OUTPUT,
    };

    enum class pullMode_t : uint32_t
    {
        NONE,
        UP,
        DOWN = NONE
    };

    /// Structure used to define single MCU pin.
    struct pin_t
    {
        pinPort_t  port;
        pinIndex_t index;
        pinMode_t  mode;
    };
}    // namespace core::mcu::io

// redefine port symbols so they can be stored as integers instead of pointers
// do not use directly!
#define PORT_A (0x02 + __SFR_OFFSET)
#define PORT_B (0x05 + __SFR_OFFSET)
#define PORT_C (0x08 + __SFR_OFFSET)
#define PORT_D (0x0B + __SFR_OFFSET)
#define PORT_E (0x0E + __SFR_OFFSET)
#define PORT_F (0x11 + __SFR_OFFSET)
#define PORT_G (0x14 + __SFR_OFFSET)
#define PORT_H (0x102)
#define PORT_J (0x105)
#define PORT_K (0x108)
#define PORT_L (0x10B)

#define CORE_MCU_IO_PORT_TO_MEM(port) (*reinterpret_cast<volatile uint8_t*>(port))

/// Workaround to avoid using DDR and PIN registers.
/// On most AVR models, DDR register is used to define pin direction within port (input or output),
/// and PIN port is used to read state of pin within port. To avoid defining DDR, PORT and PIN register for each
/// pin, these simple macros use the fact that DDR address = PORT address - 1 and PIN address = PORT address - 2.
/// Because of this, only PORT register must be defined, as well as specific pin.
/// Refer to specific AVR model datasheet for more info.
/// param [in]: port    AVR Port.
#define DDR(port) (port - 1)
#define PIN(port) (port - 2)

#define CORE_MCU_IO_SET_LOW(port, index)        ((CORE_MCU_IO_PORT_TO_MEM(port)) &= ~(1 << (index)))
#define CORE_MCU_IO_SET_HIGH(port, index)       ((CORE_MCU_IO_PORT_TO_MEM(port)) |= (1 << (index)))
#define CORE_MCU_IO_SET_PORT_STATE(port, state) ((CORE_MCU_IO_PORT_TO_MEM(port)) = (state))
#define CORE_MCU_IO_READ(port, index)           (((CORE_MCU_IO_PORT_TO_MEM(PIN(port))) >> (index)) & 0x01)
#define CORE_MCU_IO_READ_OUT_PORT(port)         (CORE_MCU_IO_PORT_TO_MEM(PIN(port)))
#define CORE_MCU_IO_READ_IN_PORT(port)          (CORE_MCU_IO_READ_OUT_PORT(port))
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

#define CORE_MCU_IO_TOGGLE(port, pin)        \
    do                                       \
    {                                        \
        if (CORE_IO_READ(port, pin))         \
        {                                    \
            CORE_MCU_IO_SET_LOW(port, pin);  \
        }                                    \
        else                                 \
        {                                    \
            CORE_MCU_IO_SET_HIGH(port, pin); \
        }                                    \
    } while (0)

/// Convenience macros for portable GPIO port/pin definitions across various toolchains.
#define CORE_MCU_IO_PIN_PORT_DEF(port)   (PORT_##port)
#define CORE_MCU_IO_PIN_INDEX_DEF(index) (index)

/// Convenience macro used to create pin_t structure.
#define CORE_MCU_IO_PIN_VAR(_port, _index) \
    {                                      \
        .port  = _port,                    \
        .index = _index                    \
    }

/// Macros used to retrieve either pin port or pin index from pin_t structure.
#define CORE_MCU_IO_PIN_PORT(mcuPin)  mcuPin.port
#define CORE_MCU_IO_PIN_INDEX(mcuPin) mcuPin.index

inline void CORE_MCU_IO_INIT(core::mcu::io::pinPort_t  port,
                             core::mcu::io::pinIndex_t index,
                             core::mcu::io::pinMode_t  mode,
                             core::mcu::io::pullMode_t pull = core::mcu::io::pullMode_t::NONE)
{
    if (mode == core::mcu::io::pinMode_t::INPUT)
    {
        CORE_MCU_IO_PORT_TO_MEM(DDR(port)) &= ~(1 << (index));

        if (pull == core::mcu::io::pullMode_t::UP)
        {
            CORE_MCU_IO_SET_HIGH(port, index);
        }
    }
    else
    {
        CORE_MCU_IO_PORT_TO_MEM(DDR(port)) |= (1 << (index));
    }
}
