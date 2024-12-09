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

#pragma once

#include <inttypes.h>
#include "hal.h"

namespace core::mcu::io
{
    using pinPort_t   = uint32_t;
    using pinIndex_t  = uint16_t;
    using portWidth_t = pinIndex_t;

    enum class pinMode_t : uint32_t
    {
        INPUT             = 0x00000000U,    // Input Floating Mode
        OUTPUT_PP         = 0x00000001U,    // Output Push Pull Mode
        OUTPUT_OD         = 0x00000011U,    // Output Open Drain Mode
        ALTERNATE_PP      = 0x00000002U,    // Alternate Function Push Pull Mode
        ALTERNATE_OD      = 0x00000012U,    // Alternate Function Open Drain Mode
        ANALOG            = 0x00000003U,
        IT_RISING         = 0x10110000U,    // External Interrupt Mode with Rising edge trigger detection
        IT_FALLING        = 0x10210000U,    // External Interrupt Mode with Falling edge trigger detection
        IT_RISING_FALLING = 0x10310000U     // External Interrupt Mode with Rising/Falling edge trigger detection
    };

    enum class pullMode_t : uint32_t
    {
        NONE = 0x00000000U,
        UP   = 0x00000001U,
        DOWN = 0x00000002U
    };

    enum class gpioSpeed_t : uint32_t
    {
        LOW       = 0x00000000U,    // IO works at 2 MHz
        MEDIUM    = 0x00000001U,    // range 12,5 MHz to 50 MHz
        HIGH      = 0x00000002U,    // range 25 MHz to 100 MHz
        VERY_HIGH = 0x00000003U     // range 50 MHz to 200 MHz
    };

    /// Structure used to define single MCU pin.
    struct pin_t
    {
        pinPort_t   port      = 0;
        pinIndex_t  index     = 0;
        pinMode_t   mode      = pinMode_t::INPUT;
        pullMode_t  pull      = pullMode_t::NONE;
        gpioSpeed_t speed     = gpioSpeed_t::MEDIUM;
        uint32_t    alternate = 0;

        constexpr pin_t(pinPort_t  port,
                        pinIndex_t index)
            : port(port)
            , index(index)
        {}

        constexpr pin_t(pinPort_t  port,
                        pinIndex_t index,
                        pinMode_t  mode,
                        pullMode_t pull)
            : port(port)
            , index(index)
            , mode(mode)
            , pull(pull)
        {}

        constexpr pin_t(pinPort_t   port,
                        pinIndex_t  index,
                        pinMode_t   mode,
                        pullMode_t  pull,
                        gpioSpeed_t speed,
                        uint32_t    alternate)
            : port(port)
            , index(index)
            , mode(mode)
            , pull(pull)
            , speed(speed)
            , alternate(alternate)
        {}

        pin_t() = default;
    };
}    // namespace core::mcu::io

#define CORE_MCU_IO_PORT_TO_MEM(port)           (reinterpret_cast<GPIO_TypeDef*>(port))
#define CORE_MCU_IO_SET_LOW(port, index)        (CORE_MCU_IO_PORT_TO_MEM(port)->BSRR = (uint32_t)index << 16U)
#define CORE_MCU_IO_SET_HIGH(port, index)       (CORE_MCU_IO_PORT_TO_MEM(port)->BSRR = index)
#define CORE_MCU_IO_SET_PORT_STATE(port, state) (CORE_MCU_IO_PORT_TO_MEM(port)->ODR = state)
#define CORE_MCU_IO_READ(port, index)           (CORE_MCU_IO_PORT_TO_MEM(port)->IDR & index)
#define CORE_MCU_IO_READ_OUT_PORT(port)         (CORE_MCU_IO_PORT_TO_MEM(port)->ODR)
#define CORE_MCU_IO_READ_IN_PORT(port)          (CORE_MCU_IO_PORT_TO_MEM(port)->IDR)
#define CORE_MCU_IO_SET_STATE(port, index, state) \
    do                                            \
    {                                             \
        if (state)                                \
        {                                         \
            CORE_MCU_IO_SET_HIGH(port, index);    \
        }                                         \
        else                                      \
        {                                         \
            CORE_MCU_IO_SET_LOW(port, index);     \
        }                                         \
    } while (0)

#define CORE_MCU_IO_TOGGLE(port, pin)                                                \
    do                                                                               \
    {                                                                                \
        if ((CORE_MCU_IO_PORT_TO_MEM(port)->ODR & pin) == pin)                       \
        {                                                                            \
            CORE_MCU_IO_PORT_TO_MEM(port)->BSRR = static_cast<uint32_t>(pin) << 16U; \
        }                                                                            \
        else                                                                         \
        {                                                                            \
            CORE_MCU_IO_PORT_TO_MEM(port)->BSRR = pin;                               \
        }                                                                            \
    } while (0)

/// Convenience macros for portable GPIO port/pin definitions across various toolchains.

#define CORE_MCU_IO_PIN_PORT_DEF(port)   (GPIO##port##_BASE)
#define CORE_MCU_IO_PIN_INDEX_DEF(index) (GPIO_PIN_##index)

inline void CORE_MCU_IO_CLOCK_ENABLE(core::mcu::io::pin_t pin)
{
#ifdef GPIOA
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOB
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOC
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOD
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOE
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOF
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOG
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOH
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOI
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_ENABLE();
        return;
    }
#endif
}

inline void CORE_MCU_IO_CLOCK_DISABLE(core::mcu::io::pin_t pin)
{
#ifdef GPIOA
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOB
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOC
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOD
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOE
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOF
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOG
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOH
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOI
    if (CORE_MCU_IO_PORT_TO_MEM(pin.port) == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_DISABLE();
        return;
    }
#endif
}

inline void CORE_MCU_IO_INIT(core::mcu::io::pin_t pin)
{
    CORE_MCU_IO_CLOCK_ENABLE(pin);

    GPIO_InitTypeDef gpioStruct = {};
    gpioStruct.Pin              = pin.index;
    gpioStruct.Mode             = static_cast<uint32_t>(pin.mode);
    gpioStruct.Pull             = static_cast<uint32_t>(pin.pull);
    gpioStruct.Speed            = static_cast<uint32_t>(pin.speed);
    gpioStruct.Alternate        = pin.alternate;

    HAL_GPIO_Init(CORE_MCU_IO_PORT_TO_MEM(pin.port), &gpioStruct);
}

inline void CORE_MCU_IO_DEINIT(core::mcu::io::pin_t pin)
{
    HAL_GPIO_DeInit(CORE_MCU_IO_PORT_TO_MEM(pin.port), pin.index);
}

inline void CORE_MCU_IO_INIT(core::mcu::io::pinPort_t  port,
                             core::mcu::io::pinIndex_t index,
                             core::mcu::io::pinMode_t  mode = core::mcu::io::pinMode_t::INPUT,
                             core::mcu::io::pullMode_t pull = core::mcu::io::pullMode_t::NONE)
{
    core::mcu::io::pin_t pin;

    pin.port  = port;
    pin.index = index;
    pin.mode  = mode;
    pin.pull  = pull;

    CORE_MCU_IO_INIT(pin);
}

inline void CORE_MCU_IO_DEINIT(core::mcu::io::pinPort_t  port,
                               core::mcu::io::pinIndex_t index)
{
    core::mcu::io::pin_t pin;

    pin.port  = port;
    pin.index = index;

    CORE_MCU_IO_DEINIT(pin);
}
