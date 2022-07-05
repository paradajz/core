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

#include "BootloaderCache.h"
#include "Bootloader.h"
#include <MCU.h>

namespace
{
    constexpr size_t PAGE_BUFFER_SIZE_WORDS = 1024;

    uint32_t _pageBuffer[PAGE_BUFFER_SIZE_WORDS];
    uint32_t _pageBufferCounter;
    uint32_t _commitStartAddress;
}    // namespace

namespace core::mcu::bootloader
{
    void resetCache()
    {
        _pageBufferCounter  = 0;
        _commitStartAddress = 0xFFFFFFFF;
    }

    void fillCache(size_t index, uint32_t addressInPage, uint32_t data)
    {
        if (_commitStartAddress == 0xFFFFFFFF)
        {
            _commitStartAddress = addressInPage;
        }

        _pageBuffer[_pageBufferCounter++] = data;

        if (_pageBufferCounter >= PAGE_BUFFER_SIZE_WORDS)
        {
            flushCache(index);
        }
    }

    void flushCache(size_t index)
    {
        for (size_t i = 0; i < _pageBufferCounter; i++)
        {
            core::mcu::flash::write32(CORE_MCU_FLASH_PAGE_ADDRESS(index) + _commitStartAddress + (i * 4),
                                      _pageBuffer[i]);
        }

        resetCache();
    }
}    // namespace core::mcu::bootloader
