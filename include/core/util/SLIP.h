/*
    Copyright 2017-2023 Igor Petrovic

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

#include <inttypes.h>

namespace core::util
{
    class SLIP
    {
        public:
        static constexpr uint8_t END     = 0xC0;
        static constexpr uint8_t ESC     = 0xDB;
        static constexpr uint8_t ESC_END = 0xDC;
        static constexpr uint8_t ESC_ESC = 0xDD;

        class ReaderBase
        {
            public:
            virtual bool read(uint8_t& data) = 0;
        };

        class WriterBase
        {
            public:
            virtual bool write(uint8_t data) = 0;
        };

        class PacketReader;

        class Packet
        {
            public:
            Packet(uint8_t* payload, size_t payloadSize)
                : _payload(payload)
                , _payloadCapacity(payloadSize)
                , _payloadLength(payloadSize)
            {}

            uint8_t* payload() const
            {
                return _payload;
            }

            size_t payloadCapacity() const
            {
                return _payloadCapacity;
            }

            size_t payloadLength() const
            {
                return _payloadLength;
            }

            uint8_t operator[](size_t index) const
            {
                return _payload[index];
            }

            Packet& operator=(const Packet& packet)
            {
                for (size_t i = 0; i < packet._payloadLength; i++)
                {
                    _payload[i] = packet._payload[i];
                }

                _payloadLength    = packet._payloadLength;
                _escapeProcessing = packet._escapeProcessing;

                return *this;
            }

            protected:
            bool setLength(size_t length)
            {
                if (length <= _payloadCapacity)
                {
                    _payloadLength = length;
                    return true;
                }

                return false;
            }

            private:
            friend class PacketReader;

            uint8_t*     _payload = nullptr;
            const size_t _payloadCapacity;
            size_t       _payloadLength    = 0;
            size_t       _readCounter      = 0;    // for reading only
            bool         _escapeProcessing = false;
        };

        class PacketReader
        {
            public:
            PacketReader(ReaderBase& reader)
                : _reader(reader)
            {}

            bool read(Packet& packet)
            {
                uint8_t data = 0;

                while (_reader.read(data))
                {
                    if (append(packet, data))
                    {
                        return true;
                    }
                }

                return false;
            }

            private:
            friend class SLIP;

            bool append(Packet& packet, uint8_t data)
            {
                if (packet.payload() == nullptr)
                {
                    return false;
                }

                switch (data)
                {
                case SLIP::END:
                {
                    packet._payloadLength = packet._readCounter;
                    return true;
                }
                break;

                case SLIP::ESC:
                {
                    if (!packet._escapeProcessing)
                    {
                        packet._escapeProcessing = true;

                        // proceed with packet assembly from the next received byte
                        return false;
                    }

                    // two escape bytes in a row should never occur
                    clear(packet);
                    return false;
                }
                break;

                case SLIP::ESC_END:
                {
                    if (packet._escapeProcessing)
                    {
                        packet._escapeProcessing = false;
                        data                     = SLIP::END;
                    }
                }
                break;

                case SLIP::ESC_ESC:
                {
                    if (packet._escapeProcessing)
                    {
                        packet._escapeProcessing = false;
                        data                     = SLIP::ESC;
                    }
                }
                break;

                default:
                    break;
                }

                if (packet._readCounter < packet._payloadCapacity)
                {
                    packet._payload[packet._readCounter++] = data;
                }

                return false;
            }

            void clear(SLIP::Packet& packet)
            {
                packet._readCounter      = 0;
                packet._escapeProcessing = false;
            }

            ReaderBase& _reader;
        };

        class PacketWriter
        {
            public:
            PacketWriter(WriterBase& writer)
                : _writer(writer)
            {}

            bool flush()
            {
                return _writer.write(SLIP::ESC);
            }

            bool write(const Packet& packet)
            {
                if (packet.payload() == nullptr)
                {
                    return false;
                }

                auto writeByte = [this](uint8_t byte)
                {
                    switch (byte)
                    {
                    case SLIP::END:
                    {
                        if (!_writer.write(SLIP::ESC))
                        {
                            return false;
                        }

                        if (!_writer.write(SLIP::ESC_END))
                        {
                            return false;
                        }
                    }
                    break;

                    case SLIP::ESC:
                    {
                        if (!_writer.write(SLIP::ESC))
                        {
                            return false;
                        }

                        if (!_writer.write(SLIP::ESC_ESC))
                        {
                            return false;
                        }
                    }
                    break;

                    default:
                    {
                        if (!_writer.write(byte))
                        {
                            return false;
                        }
                    }
                    break;
                    }

                    return true;
                };

                // start the packet with END byte to cancel out line noise if present
                if (!_writer.write(SLIP::END))
                {
                    return false;
                }

                // payload
                for (size_t i = 0; i < packet.payloadLength(); i++)
                {
                    if (!writeByte(packet[i]))
                    {
                        return false;
                    }
                }

                // end boundary
                if (!_writer.write(SLIP::END))
                {
                    return false;
                }

                return true;
            }

            private:
            WriterBase& _writer;
        };

        SLIP() = default;
    };
}    // namespace core::util
