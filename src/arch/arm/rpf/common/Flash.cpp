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

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include "hardware/flash.h"
#include "core/src/arch/common/Flash.h"
#include <MCU.h>
#include "Atomic.h"

namespace
{
    constexpr uint32_t PAGE_SIZE               = 256;
    constexpr uint32_t SECTOR_SIZE             = 4096;
    uint8_t            _sectorBuf[SECTOR_SIZE] = {};
    uint32_t           _lastCachedSector       = 0xFFFFFFFF;
}    // namespace

namespace core::mcu::flash
{
    bool isInRange(uint32_t address)
    {
        constexpr uint32_t MIN = CORE_MCU_FLASH_START_ADDR;
        constexpr uint32_t MAX = MIN + CORE_MCU_FLASH_SIZE;

        return (address >= CORE_MCU_FLASH_START_ADDR) && (address < MAX);
    }

    uint32_t size()
    {
        return CORE_MCU_FLASH_SIZE;
    }

    uint32_t pageSize(size_t index)
    {
        // This is actually sector size - external flash has concept of page (256-byte) only when performing writes.
        // Minimum erase size for use with pico SDK is 4k, so use that
        return SECTOR_SIZE;
    }

    bool erasePage(size_t index)
    {
        // pico sdk expects just the offset from CORE_MCU_FLASH_START_ADDR as an address for flash_range_erase
        CORE_MCU_ATOMIC_SECTION
        {
            flash_range_erase(index * pageSize(index), pageSize(index));
        }
        return true;
    }

    void writePage(size_t index)
    {
        // nothing to do here
    }

    bool write32(uint32_t address, uint32_t data)
    {
        if (!isInRange(address))
        {
            return false;
        }

        // Each write on RP2040 external flash must be done on a 256-byte boundary.
        // If the addresses in the page aren't writeable, 4k sector in which the page
        // resides must be erased first.

        const size_t   SECTOR               = (address - CORE_MCU_FLASH_START_ADDR) / SECTOR_SIZE;
        const size_t   PAGE                 = (address - CORE_MCU_FLASH_START_ADDR) / PAGE_SIZE;
        const size_t   INDEX_IN_SECTOR      = (address - CORE_MCU_FLASH_START_ADDR) % SECTOR_SIZE;
        const size_t   PAGE_WITHIN_SECTOR   = INDEX_IN_SECTOR / PAGE_SIZE;
        const uint32_t SECTOR_START_ADDRESS = SECTOR * SECTOR_SIZE;
        const uint32_t PAGE_START_ADDRESS   = PAGE * PAGE_SIZE;

        bool erase = false;

        // cache the entire sector first
        if (_lastCachedSector != SECTOR)
        {
            for (size_t i = 0; i < SECTOR_SIZE; i++)
            {
                read8(CORE_MCU_FLASH_START_ADDR + SECTOR_START_ADDRESS + i, _sectorBuf[i]);
            }

            _lastCachedSector = SECTOR;
        }

        // Erasing a sector takes a long time (~45ms) - do it only if needed.
        // Verify if the current addresses are writeable (value 0xFF).
        for (size_t i = 0; i < 4; i++)
        {
            if (_sectorBuf[INDEX_IN_SECTOR + i] == (data >> (8 * i) & static_cast<uint32_t>(0xFF)))
            {
                // same value - skip
                continue;
            }

            if (_sectorBuf[INDEX_IN_SECTOR + i] != 0xFF)
            {
                erase = true;
                break;
            }
        }

        _sectorBuf[INDEX_IN_SECTOR + 0] = data >> 0 & static_cast<uint32_t>(0xFF);
        _sectorBuf[INDEX_IN_SECTOR + 1] = data >> 8 & static_cast<uint32_t>(0xFF);
        _sectorBuf[INDEX_IN_SECTOR + 2] = data >> 16 & static_cast<uint32_t>(0xFF);
        _sectorBuf[INDEX_IN_SECTOR + 3] = data >> 24 & static_cast<uint32_t>(0xFF);

        // pico sdk expects just the offset from CORE_MCU_FLASH_START_ADDR as an address

        if (erase)
        {
            CORE_MCU_ATOMIC_SECTION
            {
                // wipe the entire sector first and write back updated values
                flash_range_erase(SECTOR_START_ADDRESS, SECTOR_SIZE);
                flash_range_program(SECTOR_START_ADDRESS, (const uint8_t*)&_sectorBuf, SECTOR_SIZE);
            }
        }
        else
        {
            CORE_MCU_ATOMIC_SECTION
            {
                // just update the page
                flash_range_program(PAGE_START_ADDRESS, (const uint8_t*)&_sectorBuf[PAGE_WITHIN_SECTOR * PAGE_SIZE], PAGE_SIZE);
            }
        }

        return true;
    }

    bool read8(uint32_t address, uint8_t& data)
    {
        if (!isInRange(address))
        {
            return false;
        }

        data = (*(volatile uint8_t*)address);
        return true;
    }

    bool read32(uint32_t address, uint32_t& data)
    {
        if (!isInRange(address))
        {
            return false;
        }

        data = (*(volatile uint32_t*)address);
        return true;
    }
}    // namespace core::mcu::flash