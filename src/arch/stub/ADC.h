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

#ifndef __CORE_STUB_ADC
#define __CORE_STUB_ADC

#include <inttypes.h>

namespace core
{
    namespace adc
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
            INT_2v56 = 2,
            INT_1v1  = 3
        };

        ///
        /// \brief Structure holding ADC prescaler and voltage reference settings.
        ///
        struct conf_t
        {
            prescaler_t prescaler;
            vRef_t      vref;
        };

        void     startConversion() __attribute__((weak));
        void     enableInterrupt() __attribute__((weak));
        void     setup(conf_t configuration) __attribute__((weak));
        void     disconnectDigitalIn(uint8_t adcChannel) __attribute__((weak));
        void     setChannel(uint8_t adcChannel) __attribute__((weak));
        uint16_t read() __attribute__((weak));
    }    // namespace adc
}    // namespace core
#endif