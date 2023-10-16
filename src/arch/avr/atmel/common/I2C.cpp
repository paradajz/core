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

#include <util/twi.h>

#ifdef TWCR

#include <inttypes.h>
#include "core/MCU.h"
#include "core/util/Util.h"

namespace
{
    constexpr uint32_t I2C_TRANSFER_TIMEOUT_MS = 10;
    uint32_t           _currentTime;

#define TIMEOUT_CHECK(register, bit)                                                \
    do                                                                              \
    {                                                                               \
        _currentTime = core::mcu::timing::ms();                                     \
        while (!core::util::BIT_READ(register, bit))                                \
        {                                                                           \
            if ((core::mcu::timing::ms() - _currentTime) > I2C_TRANSFER_TIMEOUT_MS) \
            {                                                                       \
                TWCR = 0;                                                           \
                TWDR = 0;                                                           \
                return false;                                                       \
            }                                                                       \
        }                                                                           \
    } while (0)

    inline bool startTransfer(uint8_t address)
    {
        // enable interrupt flag
        // enable start bit (set to master)
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        // wait for interrupt flag to be cleared
        TIMEOUT_CHECK(TWCR, TWINT);

        // check the value of TWI status register
        uint8_t status = TW_STATUS & 0xF8;

        if ((status != TW_START) && (status != TW_REP_START))
        {
            return false;
        }

        // send device address
        TWDR = address << 1;
        TWCR = (1 << TWINT) | (1 << TWEN);

        TIMEOUT_CHECK(TWCR, TWINT);

        status = TW_STATUS & 0xF8;

        if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK))
        {
            return false;
        }

        return true;
    }

    inline bool endTransfer()
    {
        // wait until all ongoing transmissions are stopped
        TWCR |= (1 << TWSTO);

        TIMEOUT_CHECK(TWCR, TWSTO);

        return true;
    }
}    // namespace

namespace core::mcu::i2c
{
    bool init(uint8_t channel, const Config& config)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        // no prescaling
        TWSR = 0x00;

        // use formula as per datasheet
        TWBR = ((F_CPU / static_cast<uint32_t>(config.clockSpeed)) - 16) / 2;

        // enable i2c interface
        TWCR = (1 << TWEN);

        return true;
    }

    bool deInit(uint8_t channel)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        // disable i2c interface
        TWCR = 0;

        return true;
    }

    bool write(uint8_t channel, uint8_t address, uint8_t* buffer, size_t size)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        if (!startTransfer(address))
        {
            return false;
        }

        for (size_t i = 0; i < size; i++)
        {
            TWDR = buffer[i];
            TWCR = (1 << TWINT) | (1 << TWEN);

            // wait for interrupt flag to be cleared
            TIMEOUT_CHECK(TWCR, TWINT);

            if ((TW_STATUS & 0xF8) != TW_MT_DATA_ACK)
            {
                return false;
            }
        }

        return endTransfer();
    }

    bool deviceAvailable(uint8_t channel, uint8_t address)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        bool found = false;

        if (startTransfer(address))
        {
            found = true;
        }

        if (!endTransfer())
        {
            return false;
        }

        return found;
    }
}    // namespace core::mcu::i2c

#endif