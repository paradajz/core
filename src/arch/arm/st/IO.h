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

#ifndef __CORE_ARM_ST_IO
#define __CORE_ARM_ST_IO

#include <inttypes.h>
#include "../../../general/Helpers.h"

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F401xE) || defined(STM32F401xC) || defined(STM32F411xE)
#include "stm32f4xx_hal.h"
#elif defined(STM32F031x6)
#include "stm32f0xx_hal.h"
#endif

///
/// \brief I/O macros and functions.
/// These macros are used only to wrap STM32 HAL API.
/// @{

namespace core
{
    namespace io
    {
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

        using pinPort_t   = uint32_t;
        using pinIndex_t  = uint16_t;
        using portWidth_t = pinIndex_t;

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

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        struct mcuPin_t
        {
            pinPort_t   port      = 0;
            pinIndex_t  index     = 0;
            pinMode_t   mode      = pinMode_t::INPUT;
            pullMode_t  pull      = pullMode_t::NONE;
            gpioSpeed_t speed     = gpioSpeed_t::MEDIUM;
            uint32_t    alternate = 0;
        };
    }    // namespace io
}    // namespace core

#define PORT_TO_MEM(port)                   (reinterpret_cast<GPIO_TypeDef*>(port))
#define CORE_IO_SET_LOW(port, index)        (PORT_TO_MEM(port)->BSRR = (uint32_t)index << 16U)
#define CORE_IO_SET_HIGH(port, index)       (PORT_TO_MEM(port)->BSRR = index)
#define CORE_IO_SET_PORT_STATE(port, state) (PORT_TO_MEM(port)->ODR = state)
#define CORE_IO_READ(port, index)           (PORT_TO_MEM(port)->IDR & index)
#define CORE_IO_READ_OUT_PORT(port)         (PORT_TO_MEM(port)->ODR)
#define CORE_IO_READ_IN_PORT(port)          (PORT_TO_MEM(port)->IDR)
#define CORE_IO_SET_STATE(port, index, state) \
    do                                        \
    {                                         \
        if (state)                            \
        {                                     \
            CORE_IO_SET_HIGH(port, index);    \
        }                                     \
        else                                  \
        {                                     \
            CORE_IO_SET_LOW(port, index);     \
        }                                     \
    } while (0)

#define CORE_IO_TOGGLE(port, pin)                                        \
    do                                                                   \
    {                                                                    \
        if ((PORT_TO_MEM(port)->ODR & pin) == pin)                       \
        {                                                                \
            PORT_TO_MEM(port)->BSRR = static_cast<uint32_t>(pin) << 16U; \
        }                                                                \
        else                                                             \
        {                                                                \
            PORT_TO_MEM(port)->BSRR = pin;                               \
        }                                                                \
    } while (0)

///
/// \brief Convenience macros for portable GPIO port/pin definitions across various toolchains.
/// @{

#define CORE_IO_PIN_PORT_DEF(port)   (GPIO##port##_BASE)
#define CORE_IO_PIN_INDEX_DEF(index) (GPIO_PIN_##index)

/// @}

///
/// \brief Convenience macro used to create mcuPin_t structure.
///
#define CORE_IO_MCU_PIN_VAR(_port, _index) \
    {                                      \
        .port  = _port,                    \
        .index = _index                    \
    }

///
/// \brief Macros used to retrieve either pin port or pin index from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_PORT(mcuPin)  mcuPin.port
#define CORE_IO_MCU_PIN_INDEX(mcuPin) mcuPin.index

/// @}

inline void CORE_IO_CLOCK_ENABLE(core::io::mcuPin_t pin)
{
#ifdef GPIOA
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOB
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOC
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOD
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOE
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOF
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOG
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOH
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
        return;
    }
#endif

#ifdef GPIOI
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_ENABLE();
        return;
    }
#endif
}

inline void CORE_IO_CLOCK_DISABLE(core::io::mcuPin_t pin)
{
#ifdef GPIOA
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOB
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOC
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOD
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOE
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOF
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOG
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOH
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_DISABLE();
        return;
    }
#endif

#ifdef GPIOI
    if (PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)) == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_DISABLE();
        return;
    }
#endif
}

inline void CORE_IO_INIT(core::io::mcuPin_t pin)
{
    CORE_IO_CLOCK_ENABLE(pin);

    GPIO_InitTypeDef gpioStruct = {};
    gpioStruct.Pin              = CORE_IO_MCU_PIN_INDEX(pin);
    gpioStruct.Mode             = static_cast<uint32_t>(pin.mode);
    gpioStruct.Pull             = static_cast<uint32_t>(pin.pull);
    gpioStruct.Speed            = static_cast<uint32_t>(pin.speed);
    gpioStruct.Alternate        = pin.alternate;

    HAL_GPIO_Init(PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)), &gpioStruct);
}

inline void CORE_IO_DEINIT(core::io::mcuPin_t pin)
{
    HAL_GPIO_DeInit(PORT_TO_MEM(CORE_IO_MCU_PIN_PORT(pin)), CORE_IO_MCU_PIN_INDEX(pin));
}

/// @}

#endif