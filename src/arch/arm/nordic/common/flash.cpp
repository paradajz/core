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

#include "core/arch/common/flash.h"
#include "core/error_handler.h"
#include "core/arch/arm/nordic/common/mcu.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"

NRF_FSTORAGE_DEF(nrf_fstorage_t _fstorage) = {
    .evt_handler = NULL,
    .start_addr  = reinterpret_cast<uint32_t>(__flash_start__),
    .end_addr    = core::mcu::flash::size() - 1,
};

namespace core::mcu::flash
{
    bool init()
    {
        nrf_fstorage_api_t* fsAPI = &nrf_fstorage_sd;
        CORE_ERROR_CHECK(nrf_fstorage_init(&_fstorage, fsAPI, NULL), NRF_SUCCESS);

        return true;
    }

    bool erasePage(size_t index)
    {
        CORE_ERROR_CHECK(nrf_fstorage_erase(&_fstorage,
                                            core::mcu::flash::pageAddress(index),
                                            1,
                                            NULL),
                         NRF_SUCCESS);

        while (nrf_fstorage_is_busy(&_fstorage))
        {
            sd_app_evt_wait();
        }

        return true;
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