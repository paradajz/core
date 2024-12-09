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

#include "core/util/util.h"
#include "core/arch/common/isr.h"
#include "core/arch/common/adc.h"
#include "hardware/regs/intctrl.h"
#include "hardware/adc.h"

extern "C" void isrADC()
{
    volatile uint32_t status = adc_hw->cs;

    // rp2040 has only 8.7 ENOB for ADC despite officially having 12bit resolution
    // artificially limit the resolution to 10bit instead
    uint16_t reading = adc_fifo_get() >> 2;

    if (status & ADC_CS_ERR_BITS)
    {
        // conversion error
        reading = 0xFFFF;
    }

    adc_fifo_drain();
    core::mcu::isr::adc(reading);
}

namespace core::mcu::adc
{
    void init(conf_t configuration)
    {
        adc_init();
        adc_set_clkdiv(configuration.prescaler);
    }

    void initPin(core::mcu::io::pin_t pin)
    {
        adc_gpio_init(pin.index);
    }

    void enableIt(uint8_t priority)
    {
        // only two upper bits are relevant (max 255)
        priority = core::util::CONSTRAIN(priority,
                                         static_cast<uint8_t>(0),
                                         static_cast<uint8_t>(3));

        priority <<= 6;

        adc_fifo_setup(true, true, 1, true, false);
        adc_irq_set_enabled(true);
        irq_set_exclusive_handler(ADC_IRQ_FIFO, isrADC);
        irq_set_enabled(ADC_IRQ_FIFO, true);
    }

    void disableIt()
    {
        adc_irq_set_enabled(false);
        irq_set_enabled(ADC_IRQ_FIFO, false);
    }

    void startItConversion()
    {
        hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);
    }

    void setActivePin(core::mcu::io::pin_t pin)
    {
        auto channel = core::mcu::peripherals::adcChannel(pin);
        adc_select_input(channel);
    }

    uint16_t read(core::mcu::io::pin_t pin)
    {
        setActivePin(pin);
        return adc_read();
    }
}    // namespace core::mcu::adc