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

#include "core/src/arch/arm/st/common/Peripherals.h"
#include <MCU.h>

namespace
{
    class UARTdescriptor0 : public core::mcu::peripherals::Descriptor
    {
        public:
        UARTdescriptor0() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return USART1;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_USART1_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_USART1_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // rx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(10),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF7_USART1,
            },

            // tx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(9),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF7_USART1,
            },
        };

        const IRQn_Type _irqn = USART1_IRQn;
    } _uartDescriptor0;

    class UARTdescriptor1 : public core::mcu::peripherals::Descriptor
    {
        public:
        UARTdescriptor1() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return USART2;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_USART2_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_USART2_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // rx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(3),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF7_USART2,
            },

            // tx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(2),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF7_USART2,
            },
        };

        const IRQn_Type _irqn = USART2_IRQn;
    } _uartDescriptor1;

    class UARTdescriptor2 : public core::mcu::peripherals::Descriptor
    {
        public:
        UARTdescriptor2() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return USART6;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_USART6_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_USART6_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // rx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(12),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF8_USART6,
            },

            // tx
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(11),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_PP,
                .pull      = core::mcu::io::pullMode_t::NONE,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF8_USART6,
            },
        };

        const IRQn_Type _irqn = USART6_IRQn;
    } _uartDescriptor2;

    class I2Cdescriptor0 : public core::mcu::peripherals::Descriptor
    {
        public:
        I2Cdescriptor0() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return I2C1;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_I2C1_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_I2C1_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // sda
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(B),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(7),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C1,
            },

            // scl
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(B),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(6),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C1,
            },
        };

        const IRQn_Type _irqn = static_cast<IRQn_Type>(0);
    } _i2cDescriptor0;

    class I2Cdescriptor1 : public core::mcu::peripherals::Descriptor
    {
        public:
        I2Cdescriptor1() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return I2C2;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_I2C2_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_I2C2_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // sda
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(B),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(3),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C2,
            },

            // scl
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(B),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(10),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C2,
            },
        };

        const IRQn_Type _irqn = static_cast<IRQn_Type>(0);
    } _i2cDescriptor1;

    class I2Cdescriptor2 : public core::mcu::peripherals::Descriptor
    {
        public:
        I2Cdescriptor2() = default;

        std::vector<core::mcu::io::pin_t> pins() override
        {
            return _pins;
        }

        void* interface() override
        {
            return I2C3;
        }

        IRQn_Type irqn() override
        {
            return _irqn;
        }

        void enableClock() override
        {
            __HAL_RCC_I2C3_CLK_ENABLE();
        }

        void disableClock() override
        {
            __HAL_RCC_I2C3_CLK_DISABLE();
        }

        private:
        std::vector<core::mcu::io::pin_t> _pins = {
            // sda
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(B),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(4),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C3,
            },

            // scl
            {
                .port      = CORE_MCU_IO_PIN_PORT_DEF(A),
                .index     = CORE_MCU_IO_PIN_INDEX_DEF(8),
                .mode      = core::mcu::io::pinMode_t::ALTERNATE_OD,
                .pull      = core::mcu::io::pullMode_t::UP,
                .speed     = core::mcu::io::gpioSpeed_t::VERY_HIGH,
                .alternate = GPIO_AF4_I2C3,
            },
        };

        const IRQn_Type _irqn = static_cast<IRQn_Type>(0);
    } _i2cDescriptor2;

    core::mcu::peripherals::Descriptor* _uartDescriptor[core::mcu::peripherals::MAX_UART_INTERFACES] = {
        &_uartDescriptor0,
        &_uartDescriptor1,
        &_uartDescriptor2,
    };

    core::mcu::peripherals::Descriptor* _i2cDescriptor[core::mcu::peripherals::MAX_I2C_INTERFACES] = {
        &_i2cDescriptor0,
        &_i2cDescriptor1,
        &_i2cDescriptor2
    };
}    // namespace

#include "core/src/arch/arm/st/variants/f4/common/Peripherals.cpp.include"