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

#ifndef __CORE_AVR_ADC
#define __CORE_AVR_ADC

#include <inttypes.h>
#include <avr/io.h>

#ifdef ADC
namespace core
{
    namespace adc
    {
        enum class prescaler_t : uint8_t
        {
            p128 = 128,
            p64  = 64,
            p32  = 32,
            p16  = 16
        };

        enum class vRef_t : uint8_t
        {
            aref    = 0,
            avcc    = 1,
            int2v56 = 2,
            int1v1  = 3
        };

        ///
        /// \brief Structure holding ADC prescaler and voltage reference settings.
        ///
        typedef struct
        {
            prescaler_t prescaler;
            vRef_t      vref;
        } conf_t;

        ///
        /// \brief Starts ADC conversion by setting ADSC bit in ADCSRA register.
        ///
        inline void startConversion()
        {
            ADCSRA |= (1 << ADSC);
        }

        ///
        /// \brief Enables ADC interrupts by setting ADIE bit in ADCSRA register.
        ///
        inline void enableInterrupt()
        {
            ADCSRA |= (1 << ADIE);
        }

        ///
        /// \brief Sets up ADC.
        /// @param [in] configuration Prescaler and analog voltage reference settings. See adcConf_t structure.
        ///
        inline void setup(conf_t configuration)
        {
            ADMUX  = 0x00;
            ADCSRA = 0x0;

            switch (configuration.prescaler)
            {
            case prescaler_t::p128:
                ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
                break;

            case prescaler_t::p64:
                ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
                break;

            case prescaler_t::p32:
                ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
                break;

            case prescaler_t::p16:
                ADCSRA |= (1 << ADPS2);
                break;

            default:
                // 128 as an fallback
                ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
                break;
            }

            switch (configuration.vref)
            {
            case vRef_t::aref:
                // nothing, this is default setting
                break;

            case vRef_t::avcc:
                ADMUX |= (1 << REFS0);
                break;

            case vRef_t::int2v56:
                ADMUX |= (1 << REFS0) | (1 << REFS1);
                break;

            case vRef_t::int1v1:
                ADMUX |= (1 << REFS1);
                break;

            default:
                // nothing
                break;
            }

            // enable ADC
            ADCSRA |= (1 << ADEN);
        }

        ///
        /// \brief Get ADC value from last set ADC channel.
        /// Blocking function.
        /// \returns Value from ADC registers (ADCH and ADCL).
        ///
        inline uint16_t read()
        {
            // single conversion mode
            ADCSRA |= (1 << ADSC);

            // wait until ADC conversion is complete
            while (ADCSRA & (1 << ADSC))
                ;

            return ADC;
        }

        ///
        /// \brief Disable digital input circuitry on specified ADC channel.
        /// @param[in] adcChannel   ADC Channel.
        ///
        inline void disconnectDigitalIn(uint8_t adcChannel)
        {
            if (adcChannel > 7)
            {
#ifdef DIDR2
                adcChannel -= 8;
                DIDR2 |= (1 << adcChannel);
#else
                return;
#endif
            }
            else
            {
                DIDR0 |= (1 << adcChannel);
            }
        }

        ///
        /// \brief Sets active ADC channel.
        /// @param[in] adcChannel   ADC Channel.
        ///
        inline void setChannel(uint8_t adcChannel)
        {
#if defined(ADCSRB) && defined(MUX5)
            if (adcChannel > 7)
            {
                adcChannel -= 8;
                ADCSRB |= (1 << MUX5);
            }
            else
            {
                ADCSRB &= ~(1 << MUX5);
            }
#endif

            // select ADC channel with safety mask
            ADMUX = (ADMUX & 0xF0) | (adcChannel & 0x0F);
        }
    }    // namespace adc
}    // namespace core
#endif

#endif