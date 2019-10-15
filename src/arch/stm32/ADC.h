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

#ifndef __CORE_STM32_ADC
#define __CORE_STM32_ADC

#include <inttypes.h>

namespace core
{
    namespace adc
    {
        enum class prescaler_t : uint8_t
        {
            p128 = 128,
            p64 = 64,
            p32 = 32,
            p16 = 16
        };

        enum class vRef_t : uint8_t
        {
            aref = 0,
            avcc = 1,
            int2v56 = 2,
            int1v1 = 3
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
        }

        ///
        /// \brief Enables ADC interrupts by setting ADIE bit in ADCSRA register.
        ///
        inline void enableInterrupt()
        {
        }

        ///
        /// \brief Sets up ADC.
        /// @param [in] configuration Prescaler and analog voltage reference settings. See adcConf_t structure.
        ///
        inline void setup(conf_t configuration)
        {
            
        }

        ///
        /// \brief Get ADC value from last set ADC channel.
        /// Blocking function.
        /// \returns Value from ADC registers (ADCH and ADCL).
        ///
        inline uint16_t read()
        {
            return 0;
        }

        ///
        /// \brief Disable digital input circuitry on specified ADC channel.
        /// @param[in] adcChannel   ADC Channel.
        ///
        inline void disconnectDigitalIn(uint8_t adcChannel)
        {
            
        }

        ///
        /// \brief Sets active ADC channel.
        /// @param[in] adcChannel   ADC Channel.
        ///
        inline void setChannel(uint8_t adcChannel)
        {
        }
    }    // namespace adc
}    // namespace core

#endif