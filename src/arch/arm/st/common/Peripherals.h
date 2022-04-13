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
#include <vector>
#include <cstddef>
#include "HAL.h"
#include "IO.h"

namespace core::mcu::peripherals
{
    class Descriptor
    {
        public:
        virtual std::vector<core::mcu::io::pin_t> pins()         = 0;
        virtual void*                             interface()    = 0;
        virtual IRQn_Type                         irqn()         = 0;
        virtual void                              enableClock()  = 0;
        virtual void                              disableClock() = 0;
    };

    /// Used to retrieve physical UART interface used on MCU for a given UART channel index as well
    /// as pins on which the interface is connected.
    Descriptor* uartDescriptor(uint8_t channel);

    /// Used to retrieve physical I2C interface used on MCU for a given I2C channel index as well
    /// as pins on which the interface is connected.
    Descriptor* i2cDescriptor(uint8_t channel);

    /// Used to retrieve physical USB interface used on MCU as well
    /// as pins on which the interface is connected.
    Descriptor* usbDescriptor();

    /// Used to retrieve UART channel on board for a specified UART interface.
    /// If no channels are mapped to the provided interface, return false.
    bool uartChannel(USART_TypeDef* interface, uint8_t& channel);

    /// Used to retrieve I2C channel on board for a specified UART interface.
    /// If no channels are mapped to the provided interface, return false.
    bool i2cChannel(I2C_TypeDef* interface, uint8_t& channel);
}    // namespace core::mcu::peripherals

#ifdef STM32F4
#include "core/src/arch/arm/st/variants/f4/common/Peripherals.h.include"
#else
#error "Unsupported STM32 family"
#endif
