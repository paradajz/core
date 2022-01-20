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

#ifndef __CORE_IO_IO
#define __CORE_IO_IO

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <inttypes.h>

namespace core
{
    namespace io
    {
        enum class pinMode_t : uint8_t
        {
            input,
            output
        };

        using pinPort_t  = volatile uint8_t*;
        using pinIndex_t = uint8_t;

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        typedef struct
        {
            pinPort_t  port;
            pinIndex_t index;
            pinMode_t  mode;
        } mcuPin_t;
    }    // namespace io
}    // namespace core

///
/// \brief Workaround to avoid using DDR and PIN registers.
/// On most AVR models, DDR register is used to define pin direction within port (input or output),
/// and PIN port is used to read state of pin within port. To avoid defining DDR, PORT and PIN register for each
/// pin, these simple macros use the fact that DDR address = PORT address - 1 and PIN address = PORT address - 2.
/// Because of this, only PORT register must be defined, as well as specific pin.
/// Refer to specific AVR model datasheet for more info.
/// @param [in] port    AVR Port.
/// @{

#define DDR(port) (*(&port - 1))
#define PIN(port) (*(&port - 2))

/// @}

#define CORE_IO_CONFIG(port, index, mode)       \
    do                                          \
    {                                           \
        if (mode == core::io::pinMode_t::input) \
            DDR(port) &= ~(1 << (index));       \
        else                                    \
            DDR(port) |= (1 << (index));        \
    } while (0)

#define CORE_IO_SET_LOW(port, index)  ((port) &= ~(1 << (index)))
#define CORE_IO_SET_HIGH(port, index) ((port) |= (1 << (index)))
#define CORE_IO_SET_STATE(port, index, state) \
    do                                        \
    {                                         \
        if (state)                            \
            CORE_IO_SET_HIGH(port, index);    \
        else                                  \
            CORE_IO_SET_LOW(port, index);     \
    } while (0)

#define CORE_IO_SET_PORT_STATE(port, state) ((port) = (state))

#define CORE_IO_READ(port, index) (((PIN(port)) >> (index)) & 0x01)
#define CORE_IO_READ_PORT(port)   (PIN(port))

#define CORE_IO_TOGGLE(port, pin)        \
    do                                   \
    {                                    \
        if (CORE_IO_READ(port, pin))     \
            CORE_IO_SET_LOW(port, pin);  \
        else                             \
            CORE_IO_SET_HIGH(port, pin); \
    } while (0)

///
/// \brief Convenience macros for portable GPIO port/pin definitions across various toolchains.
/// @{

#define CORE_IO_PIN_PORT_DEF(port)   (PORT##port)
#define CORE_IO_PIN_INDEX_DEF(index) (index)

/// @}

///
/// \brief Convenience macro used to create pinPort_t variable.
///
#define CORE_IO_PIN_PORT_VAR(port) (&port)

///
/// \brief Convenience macro used to create pinIndex_t variable.
///
#define CORE_IO_PIN_INDEX_VAR(index) (index)

///
/// \brief Convenience macro used to retrieve port from pinPort_t variable.
///
#define CORE_IO_PIN_PORT_VAR_GET(port) (*port)

///
/// \brief Convenience macro used to retrieve pin index from pinIndex_t variable.
///
#define CORE_IO_PIN_INDEX_VAR_GET(index) (index)

///
/// \brief Convenience macro used to create mcuPin_t structure.
///
#define CORE_IO_MCU_PIN_VAR(_port, _index)     \
    {                                          \
        .port  = CORE_IO_PIN_PORT_VAR(_port),  \
        .index = CORE_IO_PIN_INDEX_VAR(_index) \
    }

///
/// \brief Macros used to retrieve either pin port or pin index from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_VAR_PORT_GET(mcuPin) CORE_IO_PIN_PORT_VAR_GET(mcuPin.port)
#define CORE_IO_MCU_PIN_VAR_PIN_GET(mcuPin)  CORE_IO_PIN_INDEX_VAR_GET(mcuPin.index)

/// @}

#endif