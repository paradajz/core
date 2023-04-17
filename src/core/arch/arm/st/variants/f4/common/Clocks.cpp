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

#include "core/arch/common/Clocks.h"
#include "core/ErrorHandler.h"
#include "core/arch/arm/st/common/MCU.h"

namespace
{
    enum class clockSource_t : uint8_t
    {
        INTERNAL,
        EXTERNAL,
        AMOUNT
    };

    enum class externalClockFrequency_t : uint8_t
    {
        FREQ_8MHZ,
        FREQ_12MHZ,
        FREQ_16MHZ,
        FREQ_25MHZ,
        AMOUNT
    };

    enum class cpuFrequency_t : uint8_t
    {
        FREQ_84MHZ,
        FREQ_168MHZ,
        AMOUNT
    };

    struct scaleDescriptor_t
    {
        uint32_t source;
        uint32_t pllm;
        uint32_t plln;
        uint32_t pllp;
        uint32_t pllq;
        uint32_t pllr;
        uint32_t ahbDivider;
        uint32_t apb1Divider;
        uint32_t apb2Divider;
        uint32_t voltageScale;
        uint32_t flashLatency;
    };

    constexpr inline scaleDescriptor_t INTERNAL_SCALES[static_cast<uint8_t>(cpuFrequency_t::AMOUNT)] = {
        // FREQ_84MHZ
        {
            .source       = RCC_OSCILLATORTYPE_HSI,
            .pllm         = 16,
            .plln         = 336,
            .pllp         = 4,
            .pllq         = 7,
            .pllr         = 0,
            .ahbDivider   = RCC_SYSCLK_DIV1,
            .apb1Divider  = RCC_HCLK_DIV2,
            .apb2Divider  = RCC_HCLK_DIV1,
            .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
            .flashLatency = FLASH_LATENCY_2,
        },

        // FREQ_168MHZ
        {
            .source       = RCC_OSCILLATORTYPE_HSI,
            .pllm         = 16,
            .plln         = 336,
            .pllp         = 2,
            .pllq         = 7,
            .pllr         = 0,
            .ahbDivider   = RCC_SYSCLK_DIV1,
            .apb1Divider  = RCC_HCLK_DIV4,
            .apb2Divider  = RCC_HCLK_DIV2,
            .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
            .flashLatency = FLASH_LATENCY_5,
        },
    };

    constexpr inline scaleDescriptor_t EXTERNAL_SCALES[static_cast<uint8_t>(externalClockFrequency_t::AMOUNT)][static_cast<uint8_t>(cpuFrequency_t::AMOUNT)] = {
        // external: FREQ_8MHZ
        {
            // FREQ_84MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 8,
                .plln         = 336,
                .pllp         = 4,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV2,
                .apb2Divider  = RCC_HCLK_DIV1,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_2,
            },

            // FREQ_168MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 8,
                .plln         = 336,
                .pllp         = 2,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV4,
                .apb2Divider  = RCC_HCLK_DIV2,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_5,
            },
        },

        // external: FREQ_12MHZ
        {
            // FREQ_84MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 12,
                .plln         = 336,
                .pllp         = 4,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV2,
                .apb2Divider  = RCC_HCLK_DIV1,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_2,
            },

            // FREQ_168MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 12,
                .plln         = 336,
                .pllp         = 2,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV4,
                .apb2Divider  = RCC_HCLK_DIV2,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_5,
            },
        },

        // external: FREQ_16MHZ
        {
            // FREQ_84MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 16,
                .plln         = 336,
                .pllp         = 4,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV2,
                .apb2Divider  = RCC_HCLK_DIV1,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_2,
            },

            // FREQ_168MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 16,
                .plln         = 336,
                .pllp         = 2,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV4,
                .apb2Divider  = RCC_HCLK_DIV2,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_5,
            },
        },

        // external: FREQ_25MHZ
        {
            // FREQ_84MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 25,
                .plln         = 336,
                .pllp         = 4,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV2,
                .apb2Divider  = RCC_HCLK_DIV1,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_2,
            },

            // FREQ_168MHZ
            {
                .source       = RCC_OSCILLATORTYPE_HSE,
                .pllm         = 25,
                .plln         = 336,
                .pllp         = 2,
                .pllq         = 7,
                .pllr         = 0,
                .ahbDivider   = RCC_SYSCLK_DIV1,
                .apb1Divider  = RCC_HCLK_DIV4,
                .apb2Divider  = RCC_HCLK_DIV2,
                .voltageScale = PWR_REGULATOR_VOLTAGE_SCALE1,
                .flashLatency = FLASH_LATENCY_5,
            },
        },
    };
}    // namespace

#ifdef CORE_MCU_EXT_CLOCK_FREQ_MHZ
#if CORE_MCU_EXT_CLOCK_FREQ_MHZ == 8
constexpr size_t CLOCK_FREQ_ENUM_INDEX = static_cast<uint8_t>(externalClockFrequency_t::FREQ_8MHZ);
#elif CORE_MCU_EXT_CLOCK_FREQ_MHZ == 12
constexpr size_t CLOCK_FREQ_ENUM_INDEX = static_cast<uint8_t>(externalClockFrequency_t::FREQ_12MHZ);
#elif CORE_MCU_EXT_CLOCK_FREQ_MHZ == 16
constexpr size_t CLOCK_FREQ_ENUM_INDEX = static_cast<uint8_t>(externalClockFrequency_t::FREQ_16MHZ);
#elif CORE_MCU_EXT_CLOCK_FREQ_MHZ == 25
constexpr size_t CLOCK_FREQ_ENUM_INDEX = static_cast<uint8_t>(externalClockFrequency_t::FREQ_25MHZ);
#else
#error Unsupported external clock frequency or CORE_MCU_EXT_CLOCK_FREQ_MHZ undefined
#endif

constexpr size_t CORE_CLOCK_EXTERNAL = 1;
#else
constexpr size_t CORE_CLOCK_EXTERNAL   = 0;
constexpr size_t CLOCK_FREQ_ENUM_INDEX = 0;
#endif

#if CORE_MCU_CPU_FREQ_MHZ == 84
constexpr size_t CPU_FREQ_ENUM_INDEX = static_cast<uint8_t>(cpuFrequency_t::FREQ_84MHZ);
#elif CORE_MCU_CPU_FREQ_MHZ == 168
constexpr size_t CPU_FREQ_ENUM_INDEX   = static_cast<uint8_t>(cpuFrequency_t::FREQ_168MHZ);
#else
#error Unsupported CPU frequency or CORE_MCU_CPU_FREQ_MHZ undefined
#endif

namespace core::mcu::clocks
{
    void init()
    {
        RCC_OscInitTypeDef rccOscInitStruct = { 0 };
        RCC_ClkInitTypeDef rccClkInitStruct = { 0 };

        __HAL_RCC_PWR_CLK_ENABLE();

        const scaleDescriptor_t* descriptor = nullptr;

        if constexpr (CORE_CLOCK_EXTERNAL)
        {
            descriptor = &EXTERNAL_SCALES[CLOCK_FREQ_ENUM_INDEX][CPU_FREQ_ENUM_INDEX];
        }
        else
        {
            descriptor = &INTERNAL_SCALES[CPU_FREQ_ENUM_INDEX];
        }

        __HAL_PWR_VOLTAGESCALING_CONFIG(descriptor->voltageScale);

        if constexpr (CORE_CLOCK_EXTERNAL)
        {
            rccOscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
            rccOscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
            rccOscInitStruct.HSEState       = RCC_HSE_ON;
            rccOscInitStruct.HSIState       = RCC_HSI_OFF;
        }
        else
        {
            rccOscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
            rccOscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
            rccOscInitStruct.HSEState            = RCC_HSI_OFF;
            rccOscInitStruct.HSIState            = RCC_HSE_ON;
            rccOscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        }

        rccOscInitStruct.PLL.PLLState = RCC_PLL_ON;
        rccOscInitStruct.PLL.PLLM     = descriptor->pllm;
        rccOscInitStruct.PLL.PLLN     = descriptor->plln;
        rccOscInitStruct.PLL.PLLP     = descriptor->pllp;
        rccOscInitStruct.PLL.PLLQ     = descriptor->pllq;

        CORE_ERROR_CHECK(HAL_RCC_OscConfig(&rccOscInitStruct), HAL_OK);

        rccClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        rccClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
        rccClkInitStruct.AHBCLKDivider  = descriptor->ahbDivider;
        rccClkInitStruct.APB1CLKDivider = descriptor->apb1Divider;
        rccClkInitStruct.APB2CLKDivider = descriptor->apb2Divider;

        CORE_ERROR_CHECK(HAL_RCC_ClockConfig(&rccClkInitStruct, descriptor->flashLatency), HAL_OK);
    }
}    // namespace core::mcu::clocks