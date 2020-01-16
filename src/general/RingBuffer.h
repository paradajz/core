
/*
    Copyright 2017-2019 Igor Petrovic

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
        public:
        RingBuffer()
        {}

        bool isFull() const
        {
            bool value;

            ATOMIC_SECTION
            {
                value = count_ >= bufferSize;
            }

            return value;
        }

        bool isEmpty() const
        {
            bool value;

            ATOMIC_SECTION
            {
                value = !count_;
            }

            return value;
        }

        bool insert(T data)
        {
            if (isFull())
                return false;

            ATOMIC_SECTION
            {
                buffer[head] = data;
                head         = (head + 1) % bufferSize;
                count_++;
            }

            return true;
        }

        bool remove(T& result)
        {
            if (isEmpty())
                return false;

            ATOMIC_SECTION
            {
                size_t tail = (head + (bufferSize - count_)) % bufferSize;
                result      = buffer[tail];
                count_--;
            }

            return true;
        }

        bool clearLast()
        {
            if (isEmpty())
                return false;

            ATOMIC_SECTION
            {
                if (!head)
                    head = bufferSize - 1;
                else
                    head--;

                count_--;
            }

            return true;
        }

        bool peek(T& result)
        {
            if (isEmpty())
                return false;

            ATOMIC_SECTION
            {
                size_t tail = (head + (bufferSize - count_)) % bufferSize;
                result      = buffer[tail];
            }

            return true;
        }

        void reset()
        {
            ATOMIC_SECTION
            {
                head   = 0;
                count_ = 0;
            }
        }

        size_t count() const
        {
            size_t retVal;

            ATOMIC_SECTION
            {
                retVal = count_;
            }

            return retVal;
        }

        private:
        T            buffer[size] = {};
        size_t       head         = 0;
        const size_t bufferSize   = size;
        size_t       count_       = 0;
    };
}    // namespace core

/// @}

#endif