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

#include "core/arch/common/UART.h"
#include "core/MCU.h"

namespace
{
    UART_HandleTypeDef           _uartHandler[CORE_MCU_MAX_UART_INTERFACES];
    volatile bool                _transmitting[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::rxHandler_t _rxHandler[CORE_MCU_MAX_UART_INTERFACES];
    core::mcu::uart::txHandler_t _txHandler[CORE_MCU_MAX_UART_INTERFACES];
}    // namespace

namespace core::mcu::uart::hw
{
    bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler)
    {
        _uartHandler[config.channel].Instance        = static_cast<USART_TypeDef*>(core::mcu::peripherals::uartDescriptor(config.channel)->interface());
        _uartHandler[config.channel].Init.BaudRate   = config.baudRate;
        _uartHandler[config.channel].Init.WordLength = UART_WORDLENGTH_8B;
        _uartHandler[config.channel].Init.StopBits   = config.stopBits == Config::stopBits_t::ONE ? UART_STOPBITS_1 : UART_STOPBITS_2;

        if (config.parity == Config::parity_t::NO)
        {
            _uartHandler[config.channel].Init.Parity = UART_PARITY_NONE;
        }
        else if (config.parity == Config::parity_t::EVEN)
        {
            _uartHandler[config.channel].Init.Parity = UART_PARITY_EVEN;
        }
        else if (config.parity == Config::parity_t::ODD)
        {
            _uartHandler[config.channel].Init.Parity = UART_PARITY_ODD;
        }

        if (config.type == Config::type_t::RX_TX)
        {
            _uartHandler[config.channel].Init.Mode = UART_MODE_TX_RX;
        }
        else if (config.type == Config::type_t::RX)
        {
            _uartHandler[config.channel].Init.Mode = UART_MODE_RX;
        }
        else if (config.type == Config::type_t::TX)
        {
            _uartHandler[config.channel].Init.Mode = UART_MODE_TX;
        }

        _uartHandler[config.channel].Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        _uartHandler[config.channel].Init.OverSampling = UART_OVERSAMPLING_16;

        if (HAL_UART_Init(&_uartHandler[config.channel]) != HAL_OK)
        {
            return false;
        }

        if ((config.type == Config::type_t::RX_TX) || (config.type == Config::type_t::RX))
        {
            // enable rx interrupt
            __HAL_UART_ENABLE_IT(&_uartHandler[config.channel], UART_IT_RXNE);
        }

        _rxHandler[config.channel] = std::move(rxHandler);
        _txHandler[config.channel] = std::move(txHandler);

        return true;
    }

    bool deInit(const Config& config)
    {
        if (HAL_UART_DeInit(&_uartHandler[config.channel]) == HAL_OK)
        {
            _transmitting[config.channel] = false;
            return true;
        }

        return false;
    }

    void startTx(const Config& config)
    {
        if (!_transmitting[config.channel])
        {
            _transmitting[config.channel] = true;
            __HAL_UART_ENABLE_IT(&_uartHandler[config.channel], UART_IT_TXE);
        }
    }
}    // namespace core::mcu::uart::hw

void core::mcu::isr::uart(uint8_t channel)
{
    uint32_t isrflags = _uartHandler[channel].Instance->SR;
    uint32_t cr1its   = _uartHandler[channel].Instance->CR1;
    uint8_t  data     = _uartHandler[channel].Instance->DR;

    bool receiving = (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) ||
                     (((isrflags & USART_SR_ORE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET));

    bool txComplete = ((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET);
    bool txEmpty    = ((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET);

    if (receiving)
    {
        _rxHandler[channel](data);
    }
    else if (txEmpty || txComplete)
    {
        size_t  remainingBytes;
        uint8_t value;

        if (_txHandler[channel](value, remainingBytes))
        {
            _uartHandler[channel].Instance->DR = value;

            if (!remainingBytes)
            {
                __HAL_UART_ENABLE_IT(&_uartHandler[channel], UART_IT_TC);
                __HAL_UART_DISABLE_IT(&_uartHandler[channel], UART_IT_TXE);
            }
        }
        else
        {
            if (txComplete)
            {
                __HAL_UART_DISABLE_IT(&_uartHandler[channel], UART_IT_TC);
                _transmitting[channel] = false;
            }
        }
    }
}

#endif