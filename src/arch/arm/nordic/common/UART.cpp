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

#include <map>
#include "core/arch/common/UART.h"
#include "core/MCU.h"
#include "nrfx_uarte.h"

namespace
{
    std::map<uint32_t, nrf_uarte_baudrate_t> _baudRateMap = {
        {
            1200,
            NRF_UARTE_BAUDRATE_1200,
        },
        {
            2400,
            NRF_UARTE_BAUDRATE_2400,
        },
        {
            4800,
            NRF_UARTE_BAUDRATE_4800,
        },
        {
            9600,
            NRF_UARTE_BAUDRATE_9600,
        },
        {
            14400,
            NRF_UARTE_BAUDRATE_14400,
        },
        {
            19200,
            NRF_UARTE_BAUDRATE_19200,
        },
        {
            28800,
            NRF_UARTE_BAUDRATE_28800,
        },
        {
            31250,
            NRF_UARTE_BAUDRATE_31250,
        },
        {
            38400,
            NRF_UARTE_BAUDRATE_38400,
        },
        {
            56000,
            NRF_UARTE_BAUDRATE_56000,
        },
        {
            57600,
            NRF_UARTE_BAUDRATE_57600,
        },
        {
            76800,
            NRF_UARTE_BAUDRATE_76800,
        },
        {
            115200,
            NRF_UARTE_BAUDRATE_115200,
        },
        {
            230400,
            NRF_UARTE_BAUDRATE_230400,
        },
        {
            250000,
            NRF_UARTE_BAUDRATE_250000,
        },
        {
            460800,
            NRF_UARTE_BAUDRATE_460800,
        },
        {
            921600,
            NRF_UARTE_BAUDRATE_921600,
        },
        {
            1000000,
            NRF_UARTE_BAUDRATE_1000000,
        },
    };

    uint8_t       _nrfTxBuffer[CORE_MCU_MAX_UART_INTERFACES];
    uint8_t       _nrfRxBuffer[CORE_MCU_MAX_UART_INTERFACES];
    volatile bool _transmitting[CORE_MCU_MAX_UART_INTERFACES];
    nrfx_uarte_t  _uartInstance[CORE_MCU_MAX_UART_INTERFACES] = {
        NRFX_UARTE_INSTANCE(0),
        NRFX_UARTE_INSTANCE(1),
    };

    core::mcu::uart::rxHandler_t _rxHandler[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::txHandler_t _txHandler[CORE_MCU_MAX_UART_INTERFACES];

    enum class txEvent_t : uint8_t
    {
        EMPTY,
        COMPLETE,
        STOPPED
    };

    inline void checkTx(uint8_t channel, txEvent_t event)
    {
        size_t  remainingBytes;
        uint8_t value;

        if (_txHandler[channel](value, remainingBytes))
        {
            _nrfTxBuffer[channel] = value;
            nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ENDTX);
            nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXSTOPPED);
            nrf_uarte_tx_buffer_set(_uartInstance[channel].p_reg, &_nrfTxBuffer[channel], 1);
            _transmitting[channel] = true;
            nrf_uarte_task_trigger(_uartInstance[channel].p_reg, NRF_UARTE_TASK_STARTTX);
        }
        else
        {
            if (event == txEvent_t::STOPPED)
            {
                // NRF_UARTE_TASK_STOPTX already triggered or there was no transmission in the first place
                //  tx is done
                _transmitting[channel] = false;
            }
            else
            {
                // Transmitter has to be stopped by triggering STOPTX task to achieve
                // the lowest possible level of the UARTE power consumption.

                nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXSTOPPED);
                nrf_uarte_task_trigger(_uartInstance[channel].p_reg, NRF_UARTE_TASK_STOPTX);
            }
        }
    }
}    // namespace

namespace core::mcu::uart::hw
{
    bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler)
    {
        // unsupported by NRF52
        if (config.parity == Config::parity_t::ODD)
        {
            return false;
        }

        if (!_baudRateMap[config.baudRate])
        {
            return false;
        }

        CORE_MCU_IO_SET_HIGH(config.pins.tx.port, config.pins.tx.index);

        CORE_MCU_IO_INIT(config.pins.tx.port,
                         config.pins.tx.index,
                         core::mcu::io::pinMode_t::OUTPUT_PP,
                         core::mcu::io::pullMode_t::NONE);

        CORE_MCU_IO_INIT(config.pins.rx.port,
                         config.pins.rx.index,
                         core::mcu::io::pinMode_t::INPUT,
                         core::mcu::io::pullMode_t::NONE);

        nrfx_uarte_config_t nrfUartConfig = {};
        nrfUartConfig.hal_cfg             = {
                        .hwfc   = NRF_UARTE_HWFC_DISABLED,
                        .parity = config.parity ? NRF_UARTE_PARITY_INCLUDED : NRF_UARTE_PARITY_EXCLUDED,
                        .stop   = config.stopBits == Config::stopBits_t::ONE ? NRF_UARTE_STOP_ONE : NRF_UARTE_STOP_TWO
        };

        nrf_uarte_baudrate_set(_uartInstance[config.channel].p_reg, _baudRateMap[config.baudRate]);

        nrf_uarte_configure(_uartInstance[config.channel].p_reg, &nrfUartConfig.hal_cfg);
        nrf_uarte_txrx_pins_set(_uartInstance[config.channel].p_reg,
                                CORE_NRF_GPIO_PIN_MAP(config.pins.tx.port,
                                                      config.pins.tx.index),
                                CORE_NRF_GPIO_PIN_MAP(config.pins.rx.port,
                                                      config.pins.rx.index));

        nrf_uarte_int_enable(_uartInstance[config.channel].p_reg,
                             NRF_UARTE_INT_ENDRX_MASK |
                                 NRF_UARTE_INT_ENDTX_MASK |
                                 NRF_UARTE_INT_TXSTOPPED_MASK);

        NRFX_IRQ_PRIORITY_SET(nrfx_get_irq_number((void*)_uartInstance[config.channel].p_reg), IRQ_PRIORITY_UART);
        NVIC_ClearPendingIRQ(nrfx_get_irq_number((void*)_uartInstance[config.channel].p_reg));
        NRFX_IRQ_ENABLE(nrfx_get_irq_number((void*)_uartInstance[config.channel].p_reg));

        nrf_uarte_rx_buffer_set(_uartInstance[config.channel].p_reg, &_nrfRxBuffer[config.channel], 1);
        nrf_uarte_enable(_uartInstance[config.channel].p_reg);
        nrf_uarte_task_trigger(_uartInstance[config.channel].p_reg, NRF_UARTE_TASK_STARTRX);

        _rxHandler[config.channel] = std::move(rxHandler);
        _txHandler[config.channel] = std::move(txHandler);

        return true;
    }

    bool deInit(const Config& config)
    {
        nrf_uarte_int_disable(_uartInstance[config.channel].p_reg,
                              NRF_UARTE_INT_ENDRX_MASK |
                                  NRF_UARTE_INT_ENDTX_MASK |
                                  NRF_UARTE_INT_ERROR_MASK |
                                  NRF_UARTE_INT_RXTO_MASK |
                                  NRF_UARTE_INT_TXSTOPPED_MASK);

        NRFX_IRQ_DISABLE(nrfx_get_irq_number((void*)_uartInstance[config.channel].p_reg));

        nrf_uarte_disable(_uartInstance[config.channel].p_reg);

        CORE_MCU_IO_DEINIT(config.pins.tx);
        CORE_MCU_IO_DEINIT(config.pins.rx);

        return true;
    }

    void startTx(const Config& config)
    {
        // On NRF52, there is no TX Empty interrupt - only TX ready
        // interrupt, which occurs after the data has been sent.
        // In order to trigger the interrupt initially, STARTTX task must be triggered
        // and data needs to be written to internal NRF buffer.
        // Do this only if current data transfer has completed.

        if (!_transmitting[config.channel])
        {
            checkTx(config.channel, txEvent_t::COMPLETE);
        }
    }
}    // namespace core::mcu::uart::hw

void core::mcu::isr::uart(uint8_t channel)
{
    if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ERROR))
    {
        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ERROR);
        nrf_uarte_errorsrc_get_and_clear(_uartInstance[channel].p_reg);
    }
    else if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ENDRX))
    {
        // receive buffer is filled up

        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ENDRX);
        _rxHandler[channel](_nrfRxBuffer[channel]);
        nrf_uarte_task_trigger(_uartInstance[channel].p_reg, NRF_UARTE_TASK_STARTRX);
    }

    // Receiver timeout
    if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_RXTO))
    {
        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_RXTO);
    }

    //  tx empty
    if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXDRDY))
    {
        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXDRDY);
        checkTx(channel, txEvent_t::EMPTY);
    }

    // tx complete
    if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ENDTX))
    {
        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_ENDTX);
        checkTx(channel, txEvent_t::COMPLETE);
    }

    // tx stopped
    if (nrf_uarte_event_check(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXSTOPPED))
    {
        nrf_uarte_event_clear(_uartInstance[channel].p_reg, NRF_UARTE_EVENT_TXSTOPPED);

        // it's possible that something has entered the ring buffer in the mean time:
        // verify, but without triggering stop event again
        checkTx(channel, txEvent_t::STOPPED);
    }
}