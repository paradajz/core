/*

Copyright 2015-2022 Igor Petrovic

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include "core/util/Util.h"
#include "core/Timing.h"
#include "hardware/i2c.h"

namespace
{
    // Match SDA/SCL pin indexes with I2C interface:
    // RP2040 uses semi-flexible pin mapping for I2C -
    // specific pins use specific I2C interface
    i2c_inst* i2cInstance(uint8_t sda, uint8_t scl)
    {
        if ((sda == 0) && (scl == 1))
        {
            return i2c0;
        }

        if ((sda == 2) && (scl == 3))
        {
            return i2c1;
        }

        if ((sda == 4) && (scl == 5))
        {
            return i2c0;
        }

        if ((sda == 6) && (scl == 7))
        {
            return i2c1;
        }

        if ((sda == 8) && (scl == 9))
        {
            return i2c0;
        }

        if ((sda == 10) && (scl == 11))
        {
            return i2c1;
        }

        if ((sda == 12) && (scl == 13))
        {
            return i2c0;
        }

        if ((sda == 14) && (scl == 15))
        {
            return i2c1;
        }

        if ((sda == 26) && (scl == 27))
        {
            return i2c1;
        }

        if ((sda == 20) && (scl == 21))
        {
            return i2c0;
        }

        if ((sda == 18) && (scl == 19))
        {
            return i2c1;
        }

        if ((sda == 16) && (scl == 17))
        {
            return i2c0;
        }

        return nullptr;
    }

    // Array holding I2C interface for specific channel:
    // channel is specified by user, so it's possible to init I2C by
    // specifying "init I2C channel 0 with the following pins"
    // Each pin group matches with specific I2C interface.
    // Instance should be set in init().
    i2c_inst* _i2cInstanceMatched[CORE_MCU_MAX_I2C_INTERFACES] = {
        nullptr,
        nullptr,
    };
}    // namespace

namespace core::mcu::i2c
{
    bool init(uint8_t channel, const Config& config)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        auto instance                = i2cInstance(config.pins.sda.index, config.pins.scl.index);
        _i2cInstanceMatched[channel] = instance;

        i2c_init(_i2cInstanceMatched[channel], static_cast<uint32_t>(config.clockSpeed));
        gpio_set_function(config.pins.sda.index, GPIO_FUNC_I2C);
        gpio_set_function(config.pins.scl.index, GPIO_FUNC_I2C);
        gpio_pull_up(config.pins.sda.index);
        gpio_pull_up(config.pins.scl.index);

        return true;
    }

    bool deInit(uint8_t channel)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        i2c_deinit(_i2cInstanceMatched[channel]);

        return true;
    }

    bool write(uint8_t channel, uint8_t address, uint8_t* buffer, size_t size)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        return i2c_write_blocking(_i2cInstanceMatched[channel],
                                  address,
                                  buffer,
                                  size,
                                  false) > 0;

        return false;
    }

    bool deviceAvailable(uint8_t channel, uint8_t address)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        uint8_t dummy = 0;

        return i2c_read_blocking(_i2cInstanceMatched[channel],
                                 address,
                                 &dummy,
                                 1,
                                 false) >= 0;
    }
}    // namespace core::mcu::i2c