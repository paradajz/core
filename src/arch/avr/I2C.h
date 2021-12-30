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

#ifndef __CORE_AVR_I2C
#define __CORE_AVR_I2C

#include <util/twi.h>
#include <inttypes.h>

#ifndef I2C_CLOCK
#define I2C_CLOCK 100000L
#endif

namespace core
{
    namespace i2c
    {
        enum class transferType_t : uint8_t
        {
            write,
            read
        };

        inline void enable()
        {
            // no prescaling
            TWSR = 0x00;

            // use formula as per datasheet
            TWBR = ((F_CPU / I2C_CLOCK) - 16) / 2;

            // enable i2c interface
            TWCR = (1 << TWEN);
        }

        inline void disable(bool force)
        {
            // disable i2c interface
            TWCR = 0;
        }

        inline bool startComm(uint8_t address, transferType_t type)
        {
            // enable interrupt flag
            // enable start bit (set to master)
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

            // wait for interrupt flag to be cleared
            while (!(TWCR & (1 << TWINT)))
                ;

            // check the value of TWI status register
            uint8_t status = TW_STATUS & 0xF8;

            if ((status != TW_START) && (status != TW_REP_START))
                return false;

            // send device address
            TWDR = address + (uint8_t)type;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

            // wait for interrupt flag to be cleared
            while (!(TWCR & (1 << TWINT)))
                ;

            status = TW_STATUS & 0xF8;

            if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK))
                return false;

            return true;
        }

        inline void stopComm()
        {
            if (TWCR & (1 << TWEN))
            {
                // wait until all ongoing transmissions are stopped
                TWCR |= (1 << TWSTO);
                while (TWCR & (1 << TWSTO))
                    ;
            }
        }

        inline bool write(uint8_t data)
        {
            TWDR = data;
            TWCR = (1 << TWINT) | (1 << TWEN);

            // wait for interrupt flag to be cleared
            while (!(TWCR & (1 << TWINT)))
                ;

            if ((TW_STATUS & 0xF8) != TW_MT_DATA_ACK)
                return false;

            return true;
        }

        inline void read(uint8_t& data)
        {
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
            while (!(TWCR & (1 << TWINT)))
                ;

            data = TWDR;
        }
    }    // namespace i2c
}    // namespace core

#endif