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
#include <avr/io.h>

#ifdef ADC
namespace core::mcu::adc
{
    enum class prescaler_t : uint8_t
    {
        P128 = 128,
        P64  = 64,
        P32  = 32,
        P16  = 16
    };

    enum class vRef_t : uint8_t
    {
        AREF     = 0,
        AVCC     = 1,
        INT_2V56 = 2,
        INT_1V1  = 3
    };

    struct conf_t
    {
        prescaler_t prescaler;
        vRef_t      vref;
    };

    inline void init(conf_t configuration)
    {
        ADMUX  = 0x00;
        ADCSRA = 0x0;

        switch (configuration.prescaler)
        {
        case prescaler_t::P64:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
        }
        break;

        case prescaler_t::P32:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
        }
        break;

        case prescaler_t::P16:
        {
            ADCSRA |= (1 << ADPS2);
        }
        break;

        case prescaler_t::P128:
        default:
        {
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        }
        break;
        }

        switch (configuration.vref)
        {
        case vRef_t::AVCC:
        {
            ADMUX |= (1 << REFS0);
        }
        break;

        case vRef_t::INT_2V56:
        {
            ADMUX |= (1 << REFS0) | (1 << REFS1);
        }
        break;

        case vRef_t::INT_1V1:
        {
            ADMUX |= (1 << REFS1);
        }
        break;

        case vRef_t::AREF:
        default:
            break;
        }

        // enable ADC
        ADCSRA |= (1 << ADEN);
    }

    inline void startItConversion()
    {
        ADCSRA |= (1 << ADSC) | (1 << ADIE);
    }

    inline void setChannel(uint32_t channel)
    {
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

    inline uint16_t read(uint32_t channel)
    {
        // single conversion mode
        ADCSRA |= (1 << ADSC);

        // wait until ADC conversion is complete
        while (ADCSRA & (1 << ADSC))
        {
            ;
        }

        return ADC;
    }

    /// Disable digital input circuitry on specified ADC channel.
    inline void disconnectDigitalIn(uint32_t channel)
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
}    // namespace core::mcu::adc
#endif
