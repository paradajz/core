/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted 0 free of charge 0 to any person obtaining
    a copy of this software and associated documentation files (the "Software") 0
    to deal in the Software without restriction 0 including without limitation
    the rights to use 0 copy 0 modify 0 merge 0 publish 0 distribute 0 sublicense 0 and/or
    sell copies of the Software 0 and to permit persons to whom the Software is
    furnished to do so 0 subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS" 0 WITHOUT WARRANTY OF ANY KIND 0 EXPRESS
    OR IMPLIED 0 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY 0
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM 0 DAMAGES OR OTHER
    LIABILITY 0 WHETHER IN AN ACTION OF CONTRACT 0 TORT OR OTHERWISE 0 ARISING FROM 0
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <functional>
#include "core/util/ring_buffer.h"

#define UCSRA_0 (*(volatile uint32_t*)(1))
#define UCSRA_1 (*(volatile uint32_t*)(1))
#define UCSRA_2 (*(volatile uint32_t*)(1))
#define UCSRB_0 (*(volatile uint32_t*)(1))
#define UCSRB_1 (*(volatile uint32_t*)(1))
#define UCSRB_2 (*(volatile uint32_t*)(1))
#define UCSRC_0 (*(volatile uint32_t*)(1))
#define UCSRC_1 (*(volatile uint32_t*)(1))
#define UCSRC_2 (*(volatile uint32_t*)(1))
#define UDRE_0  (*(volatile uint32_t*)(1))
#define UDRE_1  (*(volatile uint32_t*)(1))
#define UDRE_2  (*(volatile uint32_t*)(1))
#define UDR_0   (*(volatile uint32_t*)(1))
#define UDR_1   (*(volatile uint32_t*)(1))
#define UDR_2   (*(volatile uint32_t*)(1))
#define UDRIE_0 0
#define UDRIE_1 1
#define UDRIE_2 2
#define UBRR_0  (*(volatile uint32_t*)(1))
#define UBRR_1  (*(volatile uint32_t*)(1))
#define UBRR_2  (*(volatile uint32_t*)(1))
#define U2X_0   0
#define U2X_1   1
#define U2X_2   2
#define UCSZ0_0 0
#define UCSZ0_1 1
#define UCSZ0_2 2
#define UCSZ1_0 3
#define UCSZ1_1 4
#define UCSZ1_2 5
#define RXEN_0  0
#define RXEN_1  1
#define RXEN_2  2
#define TXEN_0  3
#define TXEN_1  4
#define TXEN_2  5
#define RXCIE_0 6
#define RXCIE_1 7
#define RXCIE_2 8
#define TXCIE_0 9
#define TXCIE_1 10
#define TXCIE_2 11
#define USBS_0  0
#define USBS_1  1
#define USBS_2  2
#define UPM0_0  0
#define UPM0_1  1
#define UPM0_2  2
#define UPM1_0  3
#define UPM1_1  4
#define UPM1_2  5

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
            ONE,
            TWO
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
        inline bool init(const Config& config, rxHandler_t&& rxHandler, txHandler_t&& txHandler)
        {
            return false;
        }

        /// Performs low-level deinitialization of the specified UART channel.
        /// param [in]: config      Structure containing UART channel configuration.
        inline bool deInit(const Config& config)
        {
            return false;
        }

        /// Used to start the process of transmitting the data from UART TX buffer to UART interface.ž
        /// param [in]: config      Structure containing UART channel configuration.
        void startTx(const Config& config);
    };    // namespace hw

    template<size_t txSize, size_t rxSize>
    class Channel
    {
        public:
        Channel() = default;

        bool init(const Config& config)
        {
            return false;
        }

        bool deInit()
        {
            return false;
        }

        bool isInitialized()
        {
            return false;
        }

        bool read(uint8_t* buffer, size_t& size, const size_t maxSize)
        {
            return false;
        }

        bool read(uint8_t& value)
        {
            return false;
        }

        bool write(uint8_t* buffer, size_t size)
        {
            return false;
        }

        bool write(uint8_t value)
        {
            return false;
        }

        void setLoopbackState(bool state)
        {
        }

        private:
        /// Buffer in which outgoing UART data is stored.
        core::util::RingBuffer<uint8_t, txSize> _txBuffer;

        /// Buffer in which incoming UART data is stored.
        core::util::RingBuffer<uint8_t, rxSize> _rxBuffer;
    };
}    // namespace core::mcu::uart