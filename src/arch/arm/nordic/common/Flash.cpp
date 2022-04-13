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

#include "core/src/arch/common/Flash.h"
#include "core/src/ErrorHandler.h"
#include "MCU.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include <MCU.h>

NRF_FSTORAGE_DEF(nrf_fstorage_t _fstorage) = {
    .evt_handler = NULL,
    .start_addr  = CORE_FLASH_START_ADDR,
    .end_addr    = FLASH_END,
};

namespace core::mcu::flash
{
    bool init()
    {
        nrf_fstorage_api_t* fsAPI = &nrf_fstorage_sd;
        CORE_ERROR_CHECK(nrf_fstorage_init(&_fstorage, fsAPI, NULL), NRF_SUCCESS);

        return true;
    }

    bool isInRange(uint32_t address)
    {
        return address <= FLASH_END;
    }

    uint32_t size()
    {
        return FLASH_END + static_cast<uint32_t>(1);
    }

    uint32_t pageSize(size_t index)
    {
        CORE_ERROR_CHECK(index >= TOTAL_FLASH_PAGES, false);
        return FLASH_PAGE_SIZE_COMMON;
    }

    bool erasePage(size_t index)
    {
        CORE_ERROR_CHECK(nrf_fstorage_erase(&_fstorage,
                                            FLASH_PAGE_ADDRESS(index),
                                            1,
                                            NULL),
                         NRF_SUCCESS);

        while (nrf_fstorage_is_busy(&_fstorage))
        {
            sd_app_evt_wait();
        }

        return true;
    }

    void writePage(size_t index)
    {
        // nothing to do here
    }

    bool write16(uint32_t address, uint16_t data)
    {
        CORE_ERROR_CHECK(nrf_fstorage_write(&_fstorage,
                                            address,
                                            &data,
                                            sizeof(data),
                                            NULL),
                         NRF_SUCCESS);

        while (nrf_fstorage_is_busy(&_fstorage))
        {
            sd_app_evt_wait();
        }

        return (*(volatile uint16_t*)address) == data;
    }

    bool write32(uint32_t address, uint32_t data)
    {
        CORE_ERROR_CHECK(nrf_fstorage_write(&_fstorage,
                                            address,
                                            &data,
                                            sizeof(data),
                                            NULL),
                         NRF_SUCCESS);

        while (nrf_fstorage_is_busy(&_fstorage))
        {
            sd_app_evt_wait();
        }

        return (*(volatile uint32_t*)address) == data;
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