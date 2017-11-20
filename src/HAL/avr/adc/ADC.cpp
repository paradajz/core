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

#if (defined(USE_ADC) || defined(__DOXYGEN__)) && defined(__AVR__)

#include "ADC.h"

void setUpADC()
{
    ADMUX = 0x00;
    ADCSRA = 0x0;

    //setup adc prescaler
    #if ADC_PRESCALER == 128
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    #elif ADC_PRESCALER == 64
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
    #elif ADC_PRESCALER == 32
    ADCSRA |= (1<<ADPS2)|(1<<ADPS0);
    #elif ADC_PRESCALER == 16
    ADCSRA |= (1<<ADPS2);
    #else
    #error Incorrect ADC prescaler setting. Valid values are 128, 64, 32 and 16.
    #endif

    //setup voltage reference
    #ifdef VREF_AREF
    //nothing, this is default setting
    #elif defined(VREF_AVCC)
    ADMUX |= (1<<REFS0);
    #elif defined(VREF_INTERNAL_2V56)
    ADMUX |= (1<<REFS0) | (1<<REFS1);
    #elif defined(VREF_INTERNAL_1V1)
    ADMUX |= (1<<REFS1);
    #else
    #error "No ADC reference selected or setting is invalid. Valid options are VREF_AREF, VREF_AVCC, VREF_INTERNAL_2V56 and VREF_INTERNAL_1V1"
    #endif

    //enable ADC
    ADCSRA |= (1<<ADEN);
}

#endif