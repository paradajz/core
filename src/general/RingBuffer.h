
/*
    Copyright 2017-2021 Igor Petrovic

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

#ifndef __CORE_GENERAL_RING_BUFFER
#define __CORE_GENERAL_RING_BUFFER

#include <inttypes.h>
#include <stddef.h>
#include "Atomic.h"

namespace core
{
    template<typename T, size_t size>
    class RingBuffer
    {
        static_assert(size && !(size & (size - 1)), "Ring buffer size must be a power of two.");

        public:
        RingBuffer() = default;

        size_t count() const
        {
            size_t result = 0;

            ATOMIC_SECTION
            {
                if (_head >= _tail)
                {
                    result = _head - _tail;
                }
                else
                {
                    result = BUFFER_SIZE + _head - _tail;
                }
            }

            return result;
        }

        bool isEmpty() const
        {
            return _head == _tail;
        }

        bool isFull() const
        {
            return count() == (BUFFER_SIZE - 1);
        }

        bool insert(T data)
        {
            size_t next = (_head + 1) & (BUFFER_SIZE - 1);

            if (_tail == next)
            {
                return false;
            }

            _buffer[next] = data;
            _head         = next;

            return true;
        }

        bool remove(T& result)
        {
            if (isEmpty())
            {
                return false;
            }

            size_t next = _tail + 1;

            if (next >= BUFFER_SIZE)
            {
                next = 0;
            }

            result = _buffer[next];
            _tail  = next;

            return true;
        }

        void reset()
        {
            _head = _tail;
        }

        private:
        static constexpr size_t BUFFER_SIZE   = size;
        volatile T              _buffer[size] = {};
        volatile size_t         _head         = 0;
        volatile size_t         _tail         = 0;
    };
}    // namespace core

/// @}

#endif