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

#pragma once

#ifndef CORE_MCU_STUB

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include <inttypes.h>
#include <functional>
#include "core/util/RingBuffer.h"
#include "core/MCU.h"
#include "core/Timing.h"

namespace core::mcu::uart
{
    using rxHandler_t = std::function<void(uint8_t data)>;
    using txHandler_t = std::function<bool(uint8_t& data, size_t& remainingBytes)>;

    class Config
    {
        public:
        enum parity_t : uint8_t
        {
            NO,
            EVEN,
            ODD
        };

        enum stopBits_t : uint8_t
        {
            ONE = 1,
            TWO = 2
        };

        enum type_t : uint8_t
        {
            RX_TX,
            RX,
            TX
        };

        struct pins_t
        {
            core::mcu::io::pin_t rx;
            core::mcu::io::pin_t tx;
        };

        Config(uint8_t    channel,
               uint32_t   baudRate,
               parity_t   parity,
               stopBits_t stopBits,
               type_t     type,
               pins_t     pins)
            : channel(channel)
            , baudRate(baudRate)
            , parity(parity)
            , stopBits(stopBits)
            , type(type)
            , pins(pins)
        {}

        Config(uint8_t    channel,
               uint32_t   baudRate,
               parity_t   parity,
               stopBits_t stopBits,
               type_t     type)
            : channel(channel)
            , baudRate(baudRate)
            , parity(parity)
            , stopBits(stopBits)
            , type(type)
            , pins({})
        {}

        Config()
            : channel(0)
            , baudRate(9600)
            , parity(parity_t::NO)
            , stopBits(stopBits_t::ONE)
            , type(type_t::RX_TX)
            , pins({})
        {}

        uint8_t    channel;
        uint32_t   baudRate;
        parity_t   parity;
        stopBits_t stopBits;
        type_t     type;
        pins_t     pins;
    };

    namespace hw
    {
        /// Performs low-level initialization of the specified UART channel.
        /// param [in]: config      Structure containing UART channel configuration.
        /// param [in]: rxHandler   Function which will be called from hardware UART interrupt when new data is received.
        /// param [in]: txHandler   Function which will be called from hardware UART interrupt when new data needs to be sent.
        bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler);

        /// Performs low-level deinitialization of the specified UART channel.
        /// param [in]: config      Structure containing UART channel configuration.
        bool deInit(const Config& config);

        /// Used to start the process of transmitting the data from UART TX buffer to UART interface.ž
        /// param [in]: config      Structure containing UART channel configuration.
        void startTx(const Config& config);
    };    // namespace hw

    template<size_t txSize, size_t rxSize>
    class Channel
    {
        public:
        Channel() = default;

        /// Initializes UART peripheral.
        /// returns: True if initialization is successful, false otherwise.
        bool init(const Config& config)
        {
            if (config.channel >= CORE_MCU_MAX_UART_INTERFACES)
            {
                return false;
            }

            if (hw::init(
                    config,
                    [this](uint8_t data)
                    {
                        storeIncomingData(data);
                    },
                    [this](uint8_t& data, size_t& remainingBytes)
                    {
                        return getNextByteToSend(data, remainingBytes);
                    }))
            {
                _initialized     = true;
                _config.channel  = config.channel;
                _config.baudRate = config.baudRate;
                _config.parity   = config.parity;
                _config.stopBits = config.stopBits;
                _config.type     = config.type;
                _config.pins     = config.pins;

                return true;
            }

            return false;
        }

        /// Deinitializes UART channel.
        bool deInit()
        {
            if (!_initialized)
            {
                return false;
            }

            if (hw::deInit(_config))
            {
                setLoopbackState(false);

                _rxBuffer.reset();
                _txBuffer.reset();

                _initialized = false;

                return true;
            }

            return false;
        }

        /// Checks if UART channel is initialized.
        bool isInitialized()
        {
            return _initialized;
        }

        /// Used to read data from RX UART buffer.
        /// param [in]: buffer      Pointer to array in which read data will be stored if available.
        /// param [in]: size        Reference to variable in which amount of read bytes will be stored.
        /// param [in]: maxSize     Maximum amount of bytes which can be stored in provided buffer.
        /// returns: True if data is available, false otherwise.
        bool read(uint8_t* buffer, size_t& size, const size_t maxSize)
        {
            size = 0;

            while (_rxBuffer.remove(buffer[size++]))
            {
                if (size >= maxSize)
                {
                    break;
                }
            }

            return size > 0;
        }

        /// Used to read single value from RX UART buffer.
        /// param [in]: value       Reference to variable in which read data will be stored if available.
        /// returns: True if data is available, false otherwise.
        bool read(uint8_t& value)
        {
            value = 0;

            return _rxBuffer.remove(value);
        }

        /// Used to write data to UART TX buffer.
        /// param [in]: buffer      Pointer to array holding data to send.
        /// param [in]: size        Amount of bytes in provided buffer.
        /// returns: True on success, false otherwise
        bool write(uint8_t* buffer, size_t size)
        {
            for (size_t i = 0; i < size; i++)
            {
                while (!_txBuffer.insert(buffer[i]))
                {
                    ;
                }

                hw::startTx(_config);
            }

            return true;
        }

        /// Used to write single value to UART TX buffer.
        /// param [in]: value       Value being sent.
        /// returns: True on success, false otherwise
        bool write(uint8_t value)
        {
            return write(&value, 1);
        }

        /// Used to enable or disable UART loopback functionality.
        /// Used to pass incoming UART data to TX channel immediately.
        /// param [in]: state   New state of loopback functionality (true/enabled, false/disabled).
        void setLoopbackState(bool state)
        {
            _loopbackEnabled = state;
        }

        private:
        void storeIncomingData(uint8_t data)
        {
            if (!_loopbackEnabled)
            {
                _rxBuffer.insert(data);
            }
            else
            {
                if (_txBuffer.insert(data))
                {
                    hw::startTx(_config);
                }
            }
        }

        bool getNextByteToSend(uint8_t& data, size_t& remainingBytes)
        {
            if (_txBuffer.remove(data))
            {
                remainingBytes = _txBuffer.size();
                return true;
            }

            remainingBytes = 0;

            return false;
        }

        Config _config;

        /// Flag holding the state of UART interface (whether it's initialized or not).
        bool _initialized;

        /// Flag determining whether or not UART loopback functionality is enabled.
        /// When enabled, all incoming UART traffic is immediately passed on to UART TX.
        volatile bool _loopbackEnabled;

        /// Buffer in which outgoing UART data is stored.
        core::util::RingBuffer<uint8_t, txSize> _txBuffer;

        /// Buffer in which incoming UART data is stored.
        core::util::RingBuffer<uint8_t, rxSize> _rxBuffer;
    };
}    // namespace core::mcu::uart

#else
#include "core/arch/stub/UART.h"
#endif