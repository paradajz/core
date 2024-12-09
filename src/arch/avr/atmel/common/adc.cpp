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

#include <inttypes.h>
#include <avr/io.h>
#include "core/arch/common/adc.h"

#ifdef ADC

namespace
{
    /// Disable digital input circuitry on specified ADC channel.
    void disconnectDigitalIn(uint32_t channel)
    {
        if (channel > 7)
        {
#ifdef DIDR2
            channel -= 8;
            DIDR2 |= (1 << channel);
#else
            return;
#endif
        }
        else
        {
            DIDR0 |= (1 << channel);
        }
    }
}    // namespace

namespace core::mcu::adc
{
    void init(conf_t configuration)
    {
        ADMUX  = 0x00;
        ADCSRA = 0x0;

        switch (configuration.prescaler)
        {
        case 64:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
        }
        break;

        case 32:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
        }
        break;

        case 16:
        {
            ADCSRA |= (1 << ADPS2);
        }
        break;

        default:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        }
        break;
        }

        if (!configuration.externalRef)
        {
            ADMUX |= (1 << REFS0);
        }

        // enable ADC
        ADCSRA |= (1 << ADEN);
    }

    void initPin(core::mcu::io::pin_t pin)
    {
        CORE_MCU_IO_INIT(pin.port, pin.index, core::mcu::io::pinMode_t::ANALOG);
        CORE_MCU_IO_SET_LOW(pin.port, pin.index);

        disconnectDigitalIn(core::mcu::peripherals::adcChannel(pin));
    }

    void enableIt(uint8_t priority)
    {
        ADCSRA |= (1 << ADIE);
    }

    void disableIt()
    {
        ADCSRA &= ~(1 << ADIE);
    }

    void startItConversion()
    {
        ADCSRA |= (1 << ADSC);
    }

    void setActivePin(core::mcu::io::pin_t pin)
    {
        auto channel = core::mcu::peripherals::adcChannel(pin);

#if defined(ADCSRB) && defined(MUX5)
        if (channel > 7)
        {
            channel -= 8;
            ADCSRB |= (1 << MUX5);
        }
        else
        {
            ADCSRB &= ~(1 << MUX5);
        }
#endif

        // select ADC channel with safety mask
        ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    }

    uint16_t read(core::mcu::io::pin_t pin)
    {
        setActivePin(pin);

        // single conversion mode
        ADCSRA |= (1 << ADSC);

        // wait until ADC conversion is complete
        while (ADCSRA & (1 << ADSC))
        {
            ;
        }

        return ADC;
    }
}    // namespace core::mcu::adc

#endif