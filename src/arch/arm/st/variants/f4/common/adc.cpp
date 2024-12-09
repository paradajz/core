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
#include "stm32f4xx_hal.h"
#include "core/arch/common/adc.h"

namespace
{
    ADC_HandleTypeDef _adcHandler = {};
    uint8_t           _isrPriority;
}    // namespace

extern "C" void ADC_IRQHandler(void)
{
    core::mcu::isr::adc(ADC1->DR);
}

extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();
    }
    else
    {
        return;
    }
}

namespace core::mcu::adc
{
    void init(conf_t configuration)
    {
        switch (configuration.prescaler)
        {
        case 4:
        {
            _adcHandler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
        }
        break;

        case 8:
        {
            _adcHandler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
        }
        break;

        default:
            return;    // unsupported
        }

        _adcHandler.Instance                   = ADC1;
        _adcHandler.Init.Resolution            = ADC_RESOLUTION_12B;
        _adcHandler.Init.ScanConvMode          = DISABLE;
        _adcHandler.Init.ContinuousConvMode    = DISABLE;
        _adcHandler.Init.DiscontinuousConvMode = DISABLE;
        _adcHandler.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
        _adcHandler.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
        _adcHandler.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
        _adcHandler.Init.NbrOfConversion       = 1;
        _adcHandler.Init.DMAContinuousRequests = DISABLE;
        _adcHandler.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;

        HAL_ADC_Init(&_adcHandler);
    }

    void initPin(core::mcu::io::pin_t pin)
    {
        ADC_ChannelConfTypeDef channelConfig = {};

        CORE_MCU_IO_INIT(pin.port, pin.index, core::mcu::io::pinMode_t::ANALOG);
        CORE_MCU_IO_SET_LOW(pin.port, pin.index);

        channelConfig.Channel = core::mcu::peripherals::adcChannel(pin);
        channelConfig.Rank    = 1;

        // fine for most use cases - not too fast, not too slow
        channelConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;

        HAL_ADC_ConfigChannel(&_adcHandler, &channelConfig);
    }

    void enableIt(uint8_t priority)
    {
        HAL_NVIC_SetPriority(ADC_IRQn, 0, _isrPriority);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        HAL_ADC_Start_IT(&_adcHandler);
    }

    void disableIt()
    {
        HAL_ADC_Stop_IT(&_adcHandler);
    }

    void startItConversion()
    {
        // clear regular group conversion flag and overrun flag
        // to ensure of no unknown state from potential previous ADC operations
        ADC1->SR = ~(ADC_FLAG_EOC | ADC_FLAG_OVR);

        // enable end of conversion interrupt for regular group
        ADC1->CR1 |= (ADC_IT_EOC | ADC_IT_OVR);

        // enable the selected ADC software conversion for regular group
        ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
    }

    void setActivePin(core::mcu::io::pin_t pin)
    {
        auto channel = core::mcu::peripherals::adcChannel(pin);

        // clear the old SQx bits for the selected rank
        ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 1);

        // set the SQx bits for the selected rank
        ADC1->SQR3 |= ADC_SQR3_RK(channel, 1);
    }

    uint16_t read(core::mcu::io::pin_t pin)
    {
        setActivePin(pin);

        HAL_ADC_Start(&_adcHandler);
        HAL_ADC_PollForConversion(&_adcHandler, 1);

        return HAL_ADC_GetValue(&_adcHandler);
    }
}    // namespace core::mcu::adc