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

#include "core/ErrorHandler.h"
#include "core/MCU.h"

#define _RAM __attribute__((section(".data#"), noinline))

namespace core::mcu::flash
{
    bool init()
    {
        return true;    // nothing to do
    }

    bool isInRange(uint32_t address)
    {
        return IS_FLASH_ADDRESS(address);
    }

    uint32_t size()
    {
        return CORE_MCU_FLASH_SIZE;
    }

    uint32_t pageSize(size_t index)
    {
        CORE_ERROR_CHECK(index >= CORE_MCU_TOTAL_FLASH_PAGES, false);

        return CORE_MCU_FLASH_PAGE_SIZE(index);
    }

    _RAM bool erasePage(size_t index)
    {
        FLASH_EraseInitTypeDef pEraseInit = {};

        pEraseInit.Banks        = FLASH_BANK_1;
        pEraseInit.NbSectors    = 1;
        pEraseInit.Sector       = index;
        pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;

        uint32_t          eraseStatus;
        HAL_StatusTypeDef halStatus = HAL_FLASH_Unlock();

        if (halStatus == HAL_OK)
        {
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
            halStatus = HAL_FLASHEx_Erase(&pEraseInit, &eraseStatus);
        }

        HAL_FLASH_Lock();
        return (halStatus == HAL_OK) && (eraseStatus == 0xFFFFFFFFU);
    }

    _RAM bool write32(uint32_t address, uint32_t data)
    {
        HAL_StatusTypeDef halStatus = HAL_FLASH_Unlock();

        if (halStatus == HAL_OK)
        {
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
            halStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
        }

        HAL_FLASH_Lock();
        return (halStatus == HAL_OK);
    }

    bool read8(uint32_t address, uint8_t& data)
    {
        data = (*(volatile uint8_t*)address);
        return true;
    }

    bool read16(uint32_t address, uint16_t& data)
    {
        data = (*(volatile uint16_t*)address);
        return true;
    }

    bool read32(uint32_t address, uint32_t& data)
    {
        data = (*(volatile uint32_t*)address);
        return true;
    }
}    // namespace core::mcu::flash