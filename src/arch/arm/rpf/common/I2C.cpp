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

#include "core/src/util/Util.h"
#include "core/src/Timing.h"
#include "hardware/i2c.h"

namespace
{
    i2c_inst* _i2cInstance[CORE_MCU_MAX_I2C_INTERFACES] = {
        i2c0,
        i2c1
    };
}    // namespace

namespace core::mcu::i2c
{
    bool init(core::mcu::io::pin_t sda, core::mcu::io::pin_t scl, uint8_t channel, uint32_t clockSpeed)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        i2c_init(_i2cInstance[channel], clockSpeed);
        gpio_set_function(sda.index, GPIO_FUNC_I2C);
        gpio_set_function(scl.index, GPIO_FUNC_I2C);
        gpio_pull_up(sda.index);
        gpio_pull_up(scl.index);

        return true;
    }

    bool deInit(uint8_t channel)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        i2c_deinit(_i2cInstance[channel]);
        return true;
    }

    bool write(uint8_t channel, uint8_t address, uint8_t* buffer, size_t size)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        return i2c_write_blocking(_i2cInstance[channel],
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

        return i2c_read_blocking(_i2cInstance[channel],
                                 address,
                                 &dummy,
                                 1,
                                 false) >= 0;
    }
}    // namespace core::mcu::i2c