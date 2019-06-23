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

#ifndef __CORE_AVR_PIN_MANIPULATION
#define __CORE_AVR_PIN_MANIPULATION

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <inttypes.h>
#include "../Arch.h"

///
/// \brief Helper macros for easier port and pin manipulation.
/// \defgroup coreHALpinManAVR Pin manipulation
/// \ingroup coreHALavr
/// @{

///
/// \brief Workaround to avoid using DDR and PIN registers.
/// On most AVR models, DDR register is used to define pin direction within port (input or output),
/// and PIN port is used to read state of pin within port. To avoid defining DDR, PORT and PIN register for each
/// pin, these simple macros use the fact that DDR address = PORT address - 1 and PIN address = PORT address - 2.
/// Because of this, only PORT register must be defined, as well as specific pin.
/// Refer to specific AVR model datasheet for more info.
/// @param [in] port    AVR Port.
/// @{

#define DDR(port)               (*(&port-1))
#define PIN(port)               (*(&port-2))

/// @}


///
/// \brief AVR I/O macros.
/// @param [in] port    AVR Port.
/// @param [in] pin     Pin index on AVR port.
/// @{

#define setOutput(port, pin)    ((DDR(port)) |= (1 << (pin)))
#define setInput(port, pin)     ((DDR(port)) &= ~(1 << (pin)))
#define setAnalogIn(port, pin)  do \
{ \
    setInput((port), (pin)); \
    setLow((port), (pin)); \
} while (0)
#define setLow(port, pin)       ((port) &= ~(1 << (pin)))
#define setHigh(port, pin)      ((port) |= (1 << (pin)))
#define readPin(port, pin)      (((PIN(port)) >> (pin)) & 0x01)

/// @}

namespace core
{
    namespace avr
    {
        namespace pins
        {
            ///
            /// \brief Structure used to define single MCU pin.
            ///
            typedef struct
            {
                volatile uint8_t* port;
                uint8_t           pin;
            } mcuPin_t;

            ///
            /// \brief Structure used to define single PWM channel.
            ///
            typedef struct
            {
                volatile uint8_t* timer;
                volatile uint8_t* compareL;
                volatile uint8_t* compareH;
                uint8_t channel;
            } pwmChannel_t;

            inline void pwmOff(pwmChannel_t pwm)
            {
                *pwm.timer &= ~(1 << pwm.channel);
            }

            inline void pwmOn(pwmChannel_t pwm, uint16_t intensity)
            {
                *pwm.compareL = intensity & 0xFF;

                if (pwm.compareH != nullptr)
                    *pwm.compareH = (intensity >> 8) & 0xFF;

                *pwm.timer |= (1 << pwm.channel);
            }
        }
    }
}

#endif