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

#include "core/src/arch/common/UART.h"
#include "core/src/MCU.h"

namespace
{
    volatile bool                _transmitting[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::rxHandler_t _rxHandler[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::txHandler_t _txHandler[CORE_MCU_MAX_UART_INTERFACES];
}    // namespace

using namespace core::mcu::uart;

// these macros are used to avoid function calls in ISR as much as possible and to avoid
// code repetition with only the register names being different

#define _UBRR_GEN(x)  UBRR_##x
#define UBRR(x)       _UBRR_GEN(x)
#define _UDR_GEN(x)   UDR_##x
#define UDR(x)        _UDR_GEN(x)
#define _UCSRB_GEN(x) UCSRB_##x
#define UCSRB(x)      _UCSRB_GEN(x)
#define _UDRIE_GEN(x) UDRIE_##x
#define UDRIE(x)      _UDRIE_GEN(x)

#define UDRE_ISR(channel)                             \
    do                                                \
    {                                                 \
        uint8_t data;                                 \
        size_t  dummy;                                \
        if (_txHandler[channel](data, dummy))         \
        {                                             \
            UDR(channel) = data;                      \
        }                                             \
        else                                          \
        {                                             \
            UCSRB(channel) &= ~(1 << UDRIE(channel)); \
        }                                             \
    } while (0)

#define TXC_ISR(channel)                \
    do                                  \
    {                                   \
        _transmitting[channel] = false; \
    } while (0)

namespace core::mcu::uart::hw
{
    bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler)
    {
        int32_t baudCount = ((F_CPU / 8) + (config.baudRate / 2)) / config.baudRate;

        if ((baudCount & 1) && baudCount <= 4096)
        {
            switch (config.channel)
            {
            case 0:
            {
                UCSRA_0 = (1 << U2X_0);    // double speed uart
                UBRR_0  = baudCount - 1;
            }
            break;

#ifdef UCSRA_1
            case 1:
            {
                UCSRA_1 = (1 << U2X_1);    // double speed uart
                UBRR_1  = baudCount - 1;
            }
            break;
#endif

            default:
                break;
            }
        }
        else
        {
            switch (config.channel)
            {
            case 0:
            {
                UCSRA_0 = 0;
                UBRR_0  = (baudCount >> 1) - 1;
            }
            break;

#ifdef UCSRA_1
            case 1:
            {
                UCSRA_1 = 0;
                UBRR_1  = (baudCount >> 1) - 1;
            }
            break;
#endif

            default:
                break;
            }
        }

        // 8 bit data is fixed / non-configurable
        switch (config.channel)
        {
        case 0:
        {
            UCSRC_0 = (1 << UCSZ1_0) | (1 << UCSZ0_0);

            if (config.type == Config::type_t::RX_TX)
            {
                UCSRB_0 = (1 << RXEN_0) | (1 << TXEN_0) | (1 << RXCIE_0) | (1 << TXCIE_0);
            }
            else if (config.type == Config::type_t::RX)
            {
                UCSRB_0 = (1 << RXEN_0) | (1 << RXCIE_0);
            }
            else if (config.type == Config::type_t::TX)
            {
                UCSRB_0 = (1 << TXEN_0) | (1 << TXCIE_0);
            }

            if (config.stopBits == Config::stopBits_t::TWO)
            {
                UCSRC_0 |= (1 << USBS_0);
            }

            if (config.parity == Config::parity_t::EVEN)
            {
                UCSRC_0 |= (1 << UPM1_0);
            }
            else if (config.parity == Config::parity_t::ODD)
            {
                UCSRC_0 |= (1 << UPM0_0) | (1 << UPM1_0);
            }
        }
        break;

#ifdef UCSRC_1
        case 1:
        {
            UCSRC_1 = (1 << UCSZ1_1) | (1 << UCSZ0_1);

            if (config.type == Config::type_t::RX_TX)
            {
                UCSRB_1 = (1 << RXEN_1) | (1 << TXEN_1) | (1 << RXCIE_1) | (1 << TXCIE_1);
            }
            else if (config.type == Config::type_t::RX)
            {
                UCSRB_1 = (1 << RXEN_1) | (1 << RXCIE_1);
            }
            else if (config.type == Config::type_t::TX)
            {
                UCSRB_1 = (1 << TXEN_1) | (1 << TXCIE_1);
            }

            if (config.stopBits == Config::stopBits_t::TWO)
            {
                UCSRC_1 |= (1 << USBS_1);
            }

            if (config.parity == Config::parity_t::EVEN)
            {
                UCSRC_1 |= (1 << UPM1_1);
            }
            else if (config.parity == Config::parity_t::ODD)
            {
                UCSRC_1 |= (1 << UPM0_1) | (1 << UPM1_1);
            }
        }
        break;
#endif

        default:
            break;
        }

        _rxHandler[config.channel] = std::move(rxHandler);
        _txHandler[config.channel] = std::move(txHandler);

        return true;
    }

    bool deInit(const Config& config)
    {
        CORE_MCU_ATOMIC_SECTION
        {
            switch (config.channel)
            {
            case 0:
            {
                UCSRA_0 = 0;
                UCSRB_0 = 0;
                UCSRC_0 = 0;
                UBRR_0  = 0;
            }
            break;

#ifdef UCSRB_1
            case 1:
            {
                UCSRA_1 = 0;
                UCSRB_1 = 0;
                UCSRC_1 = 0;
                UBRR_1  = 0;
            }
            break;
#endif

            default:
                break;
            }

            _transmitting[config.channel] = false;
        }

        return true;
    }

    void startTx(const Config& config)
    {
        _transmitting[config.channel] = true;

        switch (config.channel)
        {
        case 0:
        {
            UCSRB_0 |= (1 << UDRIE_0);
        }
        break;

#ifdef UCSRB_1
        case 1:
        {
            UCSRB_1 |= (1 << UDRIE_1);
        }
        break;
#endif

        default:
            break;
        }
    }
}    // namespace core::mcu::uart::hw

/// ISR used to store incoming data from UART to buffer.

ISR(USART_RX_vect_0)
{
    uint8_t data = UDR_0;
    _rxHandler[0](data);
};

#ifdef UDR_1
ISR(USART_RX_vect_1)
{
    uint8_t data = UDR_1;
    _rxHandler[1](data);
};
#endif

/// ISR used to write outgoing data in buffer to UART.

ISR(USART_UDRE_vect_0)
{
    UDRE_ISR(0);
};

#ifdef USART_UDRE_vect_1
ISR(USART_UDRE_vect_1)
{
    UDRE_ISR(1);
};
#endif

/// ISR fired once the UART transmission is complete.

ISR(USART_TX_vect_0)
{
    TXC_ISR(0);
};

#ifdef USART_TX_vect_1
ISR(USART_TX_vect_1)
{
    TXC_ISR(1);
};
#endif

#endif
