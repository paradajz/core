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

#ifdef HW_SUPPORT_UART

#include "hardware/uart.h"
#include "hardware/irq.h"
#include "core/src/arch/common/UART.h"
#include "core/src/MCU.h"

namespace
{
    volatile bool _transmitting[CORE_MCU_MAX_UART_INTERFACES];

    constexpr uint8_t UART_IRQ[CORE_MCU_MAX_UART_INTERFACES] = {
        UART0_IRQ,
        UART1_IRQ
    };

    core::mcu::uart::rxHandler_t _rxHandler[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::txHandler_t _txHandler[CORE_MCU_MAX_UART_INTERFACES];

    // Match RX/TX pin indexes with UART interface:
    // RP2040 uses semi-flexible pin mapping for UART -
    // specific pins use specific UART interface
    uart_inst_t* uartInstance(uint8_t rx, uint8_t tx)
    {
        if ((rx == 1) && (tx == 0))
        {
            return uart0;
        }

        if ((rx == 5) && (tx == 4))
        {
            return uart1;
        }

        if ((rx == 9) && (tx == 8))
        {
            return uart1;
        }

        if ((rx == 13) && (tx == 12))
        {
            return uart0;
        }

        if ((rx == 17) && (tx == 16))
        {
            return uart0;
        }

        return nullptr;
    }

    // Array holding UART interface for specific channel:
    // channel is specified by user, so it's possible to init UART by
    // specifying "init UART channel 0 with the following pins"
    // Each pin group matches with specific UART interface.
    // Instance should be set in init().
    uart_inst_t* _uartInstanceMatched[CORE_MCU_MAX_UART_INTERFACES] = {
        nullptr,
        nullptr,
    };
}    // namespace

namespace core::mcu::uart::hw
{
    bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler)
    {
        auto instance = uartInstance(config.pins.rx.index, config.pins.tx.index);

        if (instance == nullptr)
        {
            return false;
        }

        _uartInstanceMatched[config.channel] = instance;

        uart_init(_uartInstanceMatched[config.channel], config.baudRate);

        gpio_set_function(config.pins.tx.index, GPIO_FUNC_UART);
        gpio_set_function(config.pins.rx.index, GPIO_FUNC_UART);
        uart_set_hw_flow(_uartInstanceMatched[config.channel], false, false);
        uart_set_format(_uartInstanceMatched[config.channel], 8, config.stopBits, static_cast<uart_parity_t>(config.parity));
        uart_set_fifo_enabled(_uartInstanceMatched[config.channel], false);
        irq_set_enabled(UART_IRQ[config.channel], true);
        uart_set_irq_enables(_uartInstanceMatched[config.channel], true, false);

        _rxHandler[config.channel] = std::move(rxHandler);
        _txHandler[config.channel] = std::move(txHandler);

        return true;
    }

    bool deInit(const Config& config)
    {
        return true;
    }

    void startTx(const Config& config)
    {
        if (!_transmitting[config.channel])
        {
            uart_set_irq_enables(_uartInstanceMatched[config.channel], true, true);
            core::mcu::isr::uart(config.channel);
        }
    }
}    // namespace core::mcu::uart::hw

void core::mcu::isr::uart(uint8_t channel)
{
    while (uart_is_readable(_uartInstanceMatched[channel]))
    {
        _rxHandler[channel](uart_getc(_uartInstanceMatched[channel]));
    }

    if (uart_is_writable(_uartInstanceMatched[channel]))
    {
        size_t  remainingBytes;
        uint8_t value;

        if (_txHandler[channel](value, remainingBytes))
        {
            _transmitting[channel] = true;
            uart_putc_raw(_uartInstanceMatched[channel], value);
        }
        else
        {
            // there is no "tx complete" event on rp2040 - if there's nothing more to send, set transmitting to false
            _transmitting[channel] = false;
            uart_set_irq_enables(_uartInstanceMatched[channel], true, false);
        }
    }
}

#endif