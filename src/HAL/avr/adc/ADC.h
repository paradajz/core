/*
    Copyright 2017-2018 Igor Petrovic

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
#include "Config.h"

///
/// \ingroup coreHALavrADC
/// @{

///
/// \brief Starts ADC conversion by setting ADSC bit in ADCSRA register.
///
#define startADCconversion() (ADCSRA |= (1<<ADSC))

///
/// \brief Enables ADC interrupts by setting ADIE bit in ADCSRA register.
///
#define adcInterruptEnable() (ADCSRA |= (1<<ADIE))

///
/// \brief Structure holding ADC prescaler and voltage reference settings.
/// See Config.h file for allowed settings.
///
typedef struct
{
    uint8_t prescaler;
    uint8_t vref;
} adcConf;

///
/// \brief Sets up ADC according to parameters specified in Config.h
/// @param [in] configuration prescaler and analog voltage reference settings. See adcConf structure.
///
inline void setUpADC(adcConf configuration)
{
    ADMUX = 0x00;
    ADCSRA = 0x0;

    switch(configuration.prescaler)
    {
        //setup adc prescaler
        case 128:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
        break;

        case 64:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
        break;

        case 32:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS0);
        break;

        case 16:
        ADCSRA |= (1<<ADPS2);
        break;

        default:
        //128 as an fallback
        ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
        break;
    }

    switch(configuration.vref)
    {
        case ADC_VREF_AREF:
        //nothing, this is default setting
        break;

        case ADC_VREF_AVCC:
        ADMUX |= (1<<REFS0);
        break;

        case ADC_VREF_INTERNAL_2V56:
        ADMUX |= (1<<REFS0) | (1<<REFS1);
        break;

        case ADC_VREF_INTERNAL_1V1:
        ADMUX |= (1<<REFS1);
        break;

        default:
        //nothing
        break;
    }

    //enable ADC
    ADCSRA |= (1<<ADEN);
}

///
/// \brief Get ADC value from last set ADC channel.
/// \returns Value from ADC registers (ADCH and ADCL).
///
inline uint16_t getADCvalue()
{
    //single conversion mode
    ADCSRA |= (1<<ADSC);

    //wait until ADC conversion is complete
    while (ADCSRA & (1<<ADSC));

    return ADC;
}

///
/// \brief Disable digital input circuitry on specified ADC channel.
/// @param[in] adcChannel   ADC Channel.
///
inline void disconnectDigitalInADC(uint8_t adcChannel)
{
    if (adcChannel > 7)
    {
        #ifdef DIDR2
        adcChannel -= 8;
        DIDR2 |= (1<<adcChannel);
        #else
        return;
        #endif
    }
    else
    {
        DIDR0 |= (1<<adcChannel);
    }
}

///
/// \brief Sets active ADC channel.
/// @param[in] adcChannel   ADC Channel.
///
inline void setADCchannel(uint8_t adcChannel)
{
    #if defined(ADCSRB) && defined(MUX5)
    if (adcChannel > 7)
    {
        adcChannel -= 8;
        ADCSRB |= (1<<MUX5);
    }
    else
    {
        ADCSRB &= ~(1<<MUX5);
    }
    #endif

    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (adcChannel & 0x0F);
}

/// @}