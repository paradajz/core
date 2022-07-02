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
#include "core/src/MCU.h"

namespace
{
    constexpr uint32_t I2C_TRANSFER_TIMEOUT_MS = 10;
    constexpr size_t   I2C_SCAN_RETRIES        = 3;
    I2C_HandleTypeDef  _i2cHandler[CORE_MCU_MAX_I2C_INTERFACES];
}    // namespace

namespace core::mcu::i2c
{
    bool init(uint8_t channel, uint32_t clockSpeed)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        _i2cHandler[channel].Instance             = static_cast<I2C_TypeDef*>(core::mcu::peripherals::i2cDescriptor(channel)->interface());
        _i2cHandler[channel].Init.ClockSpeed      = clockSpeed;
        _i2cHandler[channel].Init.DutyCycle       = I2C_DUTYCYCLE_2;
        _i2cHandler[channel].Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
        _i2cHandler[channel].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        _i2cHandler[channel].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        _i2cHandler[channel].Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

        return HAL_I2C_Init(&_i2cHandler[channel]) == HAL_OK;
    }

    bool deInit(uint8_t channel)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        return HAL_I2C_DeInit(&_i2cHandler[channel]) == HAL_OK;
    }

    bool write(uint8_t channel, uint8_t address, uint8_t* buffer, size_t size)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        return HAL_I2C_Master_Transmit(&_i2cHandler[channel],
                                       address << 1,
                                       buffer,
                                       size,
                                       I2C_TRANSFER_TIMEOUT_MS) == HAL_OK;
    }

    bool deviceAvailable(uint8_t channel, uint8_t address)
    {
        if (channel >= CORE_MCU_MAX_I2C_INTERFACES)
        {
            return false;
        }

        return HAL_I2C_IsDeviceReady(&_i2cHandler[channel],
                                     address << 1,
                                     I2C_SCAN_RETRIES,
                                     I2C_TRANSFER_TIMEOUT_MS) == HAL_OK;
    }
}    // namespace core::mcu::i2c