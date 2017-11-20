/*
    Copyright 2017 Igor Petrović

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

#if (defined(USE_ADC) && defined(__ARCH_AVR__)) || defined(__DOXYGEN__)

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
/// \brief Sets up ADC according to parameters specified in Config.h
///
void setUpADC();

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
    if (adcChannel < 6)
        DIDR0 |= (1<<adcChannel);
}

///
/// \brief Sets active ADC channel.
/// @param[in] adcChannel   ADC Channel.
///
inline void setADCchannel(uint8_t adcChannel)
{
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (adcChannel & 0x0F);
}

/// @}

#endif