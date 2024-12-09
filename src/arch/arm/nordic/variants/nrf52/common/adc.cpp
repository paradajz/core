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

#include "nrfx_saadc.h"
#include "core/arch/common/isr.h"
#include "core/util/util.h"
#include "core/arch/common/adc.h"

namespace
{
    // nrf requires setting value from ram as an buffer in which adc value will be stored
    nrf_saadc_value_t _adcValue;

    nrf_saadc_channel_config_t _channelConfig = {
        .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
        .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
        .gain       = NRF_SAADC_GAIN1_5,    // 3.0V by default
        .reference  = NRF_SAADC_REFERENCE_INTERNAL,
        .acq_time   = NRF_SAADC_ACQTIME_10US,
        .mode       = NRF_SAADC_MODE_SINGLE_ENDED,
        .burst      = NRF_SAADC_BURST_ENABLED,
    };

    inline uint32_t scaleValue(uint32_t value)
    {
        if (value > CORE_MCU_ADC_MAX_VALUE)
        {
            // invalid value
            return 0xFFFFFFFF;
        }

        if (_channelConfig.gain == NRF_SAADC_GAIN1_6)
        {
            // calculate maximum range for 3.3V and scale (difference is ~9%)
            static constexpr uint32_t ADC_MAX_RANGE_V33 =
                static_cast<uint32_t>(CORE_MCU_ADC_MAX_VALUE) - static_cast<uint32_t>(static_cast<float>(CORE_MCU_ADC_MAX_VALUE) * 0.09);

            return core::util::MAP_RANGE(value,
                                         static_cast<uint32_t>(0),
                                         ADC_MAX_RANGE_V33,
                                         static_cast<uint32_t>(0),
                                         static_cast<uint32_t>(CORE_MCU_ADC_MAX_VALUE));
        }

        return value;
    }
}    // namespace

extern "C" void SAADC_IRQHandler(void)
{
    auto value = *reinterpret_cast<nrf_saadc_value_t*>(NRF_SAADC->RESULT.PTR);

    if (value < 0)
    {
        value = 0;
    }

    if (nrf_saadc_event_check(NRF_SAADC, NRF_SAADC_EVENT_STARTED))
    {
        nrf_saadc_event_clear(NRF_SAADC, NRF_SAADC_EVENT_STARTED);
        nrf_saadc_task_trigger(NRF_SAADC, NRF_SAADC_TASK_SAMPLE);
    }

    if (nrf_saadc_event_check(NRF_SAADC, NRF_SAADC_EVENT_STOPPED))
    {
        nrf_saadc_event_clear(NRF_SAADC, NRF_SAADC_EVENT_STOPPED);
    }

    if (nrf_saadc_event_check(NRF_SAADC, NRF_SAADC_EVENT_END))
    {
        nrf_saadc_event_clear(NRF_SAADC, NRF_SAADC_EVENT_END);
        core::mcu::isr::adc(scaleValue(value));
    }

    if (nrf_saadc_event_check(NRF_SAADC, NRF_SAADC_EVENT_CALIBRATEDONE))
    {
        nrf_saadc_event_clear(NRF_SAADC, NRF_SAADC_EVENT_CALIBRATEDONE);
    }
}

namespace core::mcu::adc
{
    void init(conf_t configuration)
    {
        // if 3.3V input analog voltage is used, ADC requires gain setting of 3.6V (nothing between 3.0V and 3.6V)
        if (configuration.voltage == 33)
        {
            _channelConfig.gain = NRF_SAADC_GAIN1_6;
        }

        // otherwise just use 3.0V as default

        nrf_saadc_buffer_init(NRF_SAADC, &_adcValue, 1);
        nrf_saadc_enable(NRF_SAADC);
        nrf_saadc_task_trigger(NRF_SAADC, NRF_SAADC_TASK_CALIBRATEOFFSET);

        while (!nrf_saadc_event_check(NRF_SAADC, NRF_SAADC_EVENT_CALIBRATEDONE))
        {
        }

        nrf_saadc_event_clear(NRF_SAADC, NRF_SAADC_EVENT_CALIBRATEDONE);

        nrf_saadc_resolution_set(NRF_SAADC, NRF_SAADC_RESOLUTION_12BIT);
        nrf_saadc_oversample_set(NRF_SAADC, NRF_SAADC_OVERSAMPLE_8X);
    }

    void initPin(core::mcu::io::pin_t pin)
    {
        auto channel = core::mcu::peripherals::adcChannel(pin);
        nrf_saadc_channel_init(NRF_SAADC, channel, &_channelConfig);
    }

    void enableIt(uint8_t priority)
    {
        nrf_saadc_int_set(NRF_SAADC, NRF_SAADC_INT_STARTED | NRF_SAADC_INT_STOPPED | NRF_SAADC_INT_END);
        NRFX_IRQ_PRIORITY_SET(SAADC_IRQn, priority);
        NRFX_IRQ_ENABLE(SAADC_IRQn);
    }

    void disableIt()
    {
        NRFX_IRQ_DISABLE(SAADC_IRQn);
    }

    void startItConversion()
    {
        nrf_saadc_task_trigger(NRF_SAADC, NRF_SAADC_TASK_START);
    }

    void setActivePin(core::mcu::io::pin_t pin)
    {
        auto channel = core::mcu::peripherals::adcChannel(pin);

        for (uint32_t chPos = 0; chPos < SAADC_CH_NUM; chPos++)
        {
            nrf_saadc_input_t pselp = NRF_SAADC_INPUT_DISABLED;

            if ((1 << channel) & (1 << chPos))
            {
                pselp = static_cast<nrf_saadc_input_t>(chPos + 1);
            }

            nrf_saadc_burst_set(NRF_SAADC, chPos, NRF_SAADC_BURST_ENABLED);
            nrf_saadc_channel_input_set(NRF_SAADC, chPos, pselp, NRF_SAADC_INPUT_DISABLED);
        }
    }

    uint16_t read(core::mcu::io::pin_t pin)
    {
        setActivePin(pin);

        // TODO
        return 0xFFFF;
    }
}    // namespace core::mcu::adc