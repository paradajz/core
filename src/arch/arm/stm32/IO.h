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

#ifndef __CORE_STM32_IO
#define __CORE_STM32_IO

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
            input           = 0x00000000U,    // Input Floating Mode
            outputPP        = 0x00000001U,    // Output Push Pull Mode
            outputOD        = 0x00000011U,    // Output Open Drain Mode
            alternatePP     = 0x00000002U,    // Alternate Function Push Pull Mode
            alternateOD     = 0x00000012U,    // Alternate Function Open Drain Mode
            analog          = 0x00000003U,
            itRising        = 0x10110000U,    // External Interrupt Mode with Rising edge trigger detection
            itFalling       = 0x10210000U,    // External Interrupt Mode with Falling edge trigger detection
            itRisingFalling = 0x10310000U     // External Interrupt Mode with Rising/Falling edge trigger detection
        };

        using pinPort_t  = GPIO_TypeDef*;
        using pinIndex_t = uint16_t;

        enum class pullMode_t : uint32_t
        {
            none = 0x00000000U,
            up   = 0x00000001U,
            down = 0x00000002U
        };

        enum class gpioSpeed_t : uint32_t
        {
            low      = 0x00000000U,    // IO works at 2 MHz
            medium   = 0x00000001U,    // range 12,5 MHz to 50 MHz
            high     = 0x00000002U,    // range 25 MHz to 100 MHz
            veryHigh = 0x00000003U     // range 50 MHz to 200 MHz
        };

        ///
        /// \brief Structure used to define single MCU pin.
        ///
        typedef struct
        {
            pinPort_t   port;
            pinIndex_t  index;
            pinMode_t   mode;
            pullMode_t  pull;
            gpioSpeed_t speed;
            uint32_t    alternate;
        } mcuPin_t;
    }    // namespace io
}    // namespace core

inline void CORE_IO_CLOCK_ENABLE(GPIO_TypeDef* port)
{
#ifdef GPIOA
    if (port == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
#endif

#ifdef GPIOB
    if (port == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
#endif

#ifdef GPIOC
    if (port == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
#endif

#ifdef GPIOD
    if (port == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
#endif

#ifdef GPIOE
    if (port == GPIOE)
        __HAL_RCC_GPIOE_CLK_ENABLE();
#endif

#ifdef GPIOF
    if (port == GPIOF)
        __HAL_RCC_GPIOF_CLK_ENABLE();
#endif

#ifdef GPIOG
    if (port == GPIOG)
        __HAL_RCC_GPIOG_CLK_ENABLE();
#endif

#ifdef GPIOH
    if (port == GPIOH)
        __HAL_RCC_GPIOH_CLK_ENABLE();
#endif

#ifdef GPIOI
    if (port == GPIOI)
        __HAL_RCC_GPIOI_CLK_ENABLE();
#endif
}

inline void CORE_IO_CLOCK_DISABLE(GPIO_TypeDef* port)
{
#ifdef GPIOA
    if (port == GPIOA)
        __HAL_RCC_GPIOA_CLK_DISABLE();
#endif

#ifdef GPIOB
    if (port == GPIOB)
        __HAL_RCC_GPIOB_CLK_DISABLE();
#endif

#ifdef GPIOC
    if (port == GPIOC)
        __HAL_RCC_GPIOC_CLK_DISABLE();
#endif

#ifdef GPIOD
    if (port == GPIOD)
        __HAL_RCC_GPIOD_CLK_DISABLE();
#endif

#ifdef GPIOE
    if (port == GPIOE)
        __HAL_RCC_GPIOE_CLK_DISABLE();
#endif

#ifdef GPIOF
    if (port == GPIOF)
        __HAL_RCC_GPIOF_CLK_DISABLE();
#endif

#ifdef GPIOG
    if (port == GPIOG)
        __HAL_RCC_GPIOG_CLK_DISABLE();
#endif

#ifdef GPIOH
    if (port == GPIOH)
        __HAL_RCC_GPIOH_CLK_DISABLE();
#endif

#ifdef GPIOI
    if (port == GPIOI)
        __HAL_RCC_GPIOI_CLK_DISABLE();
#endif
}

inline void CORE_IO_CONFIG(core::io::mcuPin_t pin)
{
    CORE_IO_CLOCK_ENABLE(pin.port);

    GPIO_InitTypeDef gpioStruct = {};
    gpioStruct.Pin              = pin.index;
    gpioStruct.Mode             = static_cast<uint32_t>(pin.mode);
    gpioStruct.Pull             = static_cast<uint32_t>(pin.pull);
    gpioStruct.Speed            = static_cast<uint32_t>(pin.speed);
    gpioStruct.Alternate        = pin.alternate;

    HAL_GPIO_Init(pin.port, &gpioStruct);
}

#define CORE_IO_SET_LOW(port, index)  (port->BSRR = (uint32_t)index << 16U)
#define CORE_IO_SET_HIGH(port, index) (port->BSRR = index)
#define CORE_IO_SET_STATE(port, index, state) \
    do                                        \
    {                                         \
        if (state)                            \
            CORE_IO_SET_HIGH(port, index);    \
        else                                  \
            CORE_IO_SET_LOW(port, index);     \
    } while (0)

#define CORE_IO_SET_PORT_STATE(port, state) (port->ODR = state)

inline void CORE_IO_SET_STATE_MULTIPLE(GPIO_TypeDef* port, uint16_t bitsToChange, uint16_t value)
{
    uint32_t bssrValue = 0;

    for (size_t i = 0; i < 16; i++)
    {
        if (!BIT_READ(bitsToChange, i))
            continue;

        if (BIT_READ(value, i))
            BIT_SET(bssrValue, i);
        else
            BIT_SET(bssrValue, i + 16);
    }

    port->BSRR = bssrValue;
}

#define CORE_IO_READ(port, index) (port->IDR & index)
#define CORE_IO_READ_PORT(port)   (port->IDR)

#define CORE_IO_TOGGLE(port, pin)                           \
    do                                                      \
    {                                                       \
        if ((port->ODR & pin) == pin)                       \
            port->BSRR = static_cast<uint32_t>(pin) << 16U; \
        else                                                \
            port->BSRR = pin;                               \
    } while (0)

///
/// \brief Convenience macros for portable GPIO port/pin definitions across various toolchains.
/// @{

#define CORE_IO_PIN_PORT_DEF(port)   (GPIO##port)
#define CORE_IO_PIN_INDEX_DEF(index) (GPIO_PIN_##index)

/// @}

///
/// \brief Convenience macro used to create pinPort_t variable.
///
#define CORE_IO_PIN_PORT_VAR(port) (port)

///
/// \brief Convenience macro used to create pinIndex_t variable.
///
#define CORE_IO_PIN_INDEX_VAR(index) (index)

///
/// \brief Convenience macro used to retrieve port from pinPort_t variable.
///
#define CORE_IO_PIN_PORT_VAR_GET(port) (port)

///
/// \brief Convenience macro used to retrieve pin index from pinIndex_t variable.
///
#define CORE_IO_PIN_INDEX_VAR_GET(index) (index)

///
/// \brief Convenience macro used to create mcuPin_t structure.
///
#define CORE_IO_MCU_PIN_VAR(_port, _index)     \
    {                                          \
        .port  = CORE_IO_PIN_PORT_VAR(_port),  \
        .index = CORE_IO_PIN_INDEX_VAR(_index) \
    }

///
/// \brief Macros used to retrieve either pin port or pin index from mcuPin_t structure.
/// @{

#define CORE_IO_MCU_PIN_VAR_PORT_GET(mcuPin) CORE_IO_PIN_PORT_VAR_GET(mcuPin.port)
#define CORE_IO_MCU_PIN_VAR_PIN_GET(mcuPin)  CORE_IO_PIN_INDEX_VAR_GET(mcuPin.index)

/// @}

/// @}

#endif