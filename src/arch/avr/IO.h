/*
    Copyright 2017-2019 Igor Petrovic

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

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        typedef struct
        {
            volatile uint8_t* port;
            uint8_t           index;
            pinMode_t         mode;
        } mcuPin_t;

        ///
        /// \brief Structure used to define single PWM channel.
        ///
        typedef struct
        {
            volatile uint8_t* controlRegister;
            volatile uint8_t* compareValueL;
            volatile uint8_t* compareValueH;
            uint8_t           compareOutMode;
        } pwmChannel_t;

        inline void pwmOff(pwmChannel_t pwm)
        {
            *pwm.controlRegister &= ~pwm.compareOutMode;
        }

        inline void pwmOn(pwmChannel_t pwm, uint16_t intensity)
        {
            *pwm.compareValueL = intensity & 0xFF;

            if (pwm.compareValueH != nullptr)
                *pwm.compareValueH = (intensity >> 8) & 0xFF;

            *pwm.controlRegister |= pwm.compareOutMode;
        }
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

#define CORE_IO_SET_LOW(port, index)        ((port) &= ~(1 << (index)))
#define CORE_IO_SET_HIGH(port, index)       ((port) |= (1 << (index)))
#define CORE_IO_SET_STATE(port, index, state) do \
{ \
    if (state) \
        CORE_IO_SET_HIGH(port, index); \
    else \
        CORE_IO_SET_LOW(port, index); \
} while(0)

#define CORE_IO_READ(port, index)           (((PIN(port)) >> (index)) & 0x01)
#define CORE_IO_READ_PORT(port)             (PIN(port))

///
/// \brief Convenience macro to easily create  mcuPin_t instances.
///
#define CORE_IO_MCU_PIN_DEF(avrPort, avrPinIndex) { .port = &avrPort, .index = avrPinIndex }

///
/// \brief Macros used to retrieve either pin port or pin index from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_PORT(mcuPin)        *mcuPin.port
#define CORE_IO_MCU_PIN_INDEX(mcuPin)       mcuPin.index
/// @}

#endif