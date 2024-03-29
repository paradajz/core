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

#include <inttypes.h>
#include <cstddef>

namespace core::mcu::flash
{
    inline bool init()
    {
        return false;
    }

    inline bool isInRange(uint32_t address)
    {
        return false;
    }

    inline uint32_t startAddress()
    {
        return 0;
    }

    inline uint32_t size()
    {
        return 0;
    }

    inline uint32_t pageSize(size_t index)
    {
        return 0;
    }

    inline uint32_t pageAddress(size_t index)
    {
        return 0;
    }

    inline bool erasePage(size_t index)
    {
        return false;
    }

    inline bool write32(uint32_t address, uint32_t data)
    {
        return false;
    }

    inline bool read8(uint32_t address, uint8_t& data)
    {
        return false;
    }

    inline bool read16(uint32_t address, uint16_t& data)
    {
        return false;
    }

    inline bool read32(uint32_t address, uint32_t& data)
    {
        return false;
    }
}    // namespace core::mcu::flash