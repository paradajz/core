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

#include "core/error_handler.h"
#include "core/mcu.h"

namespace
{
    constexpr size_t NUM_OF_TIMERS = 5;

    struct f4Timer_t
    {
        enum clockSource_t : uint8_t
        {
            APB1,
            APB2
        };

        TIM_TypeDef*                 instance  = nullptr;
        IRQn_Type                    irqn      = static_cast<IRQn_Type>(0);
        TIM_HandleTypeDef            timer     = {};
        clockSource_t                source    = clockSource_t::APB1;
        uint32_t                     clock     = CORE_MCU_CPU_FREQ_MHZ * 1000000;
        bool                         supported = false;
        bool                         allocated = false;
        core::mcu::timers::handler_t handler   = nullptr;

        f4Timer_t() = default;
    };

    std::array<f4Timer_t, NUM_OF_TIMERS> _timer = {
        f4Timer_t{
#ifdef TIM6
            TIM6,
            TIM6_DAC_IRQn,
            {},
            f4Timer_t::clockSource_t::APB1,
            CORE_MCU_CPU_FREQ_MHZ * 1000000,
            true,
            false,
            nullptr,
#endif
        },

        f4Timer_t{
#ifdef TIM7
            TIM7,
            TIM7_IRQn,
            {},
            f4Timer_t::clockSource_t::APB1,
            CORE_MCU_CPU_FREQ_MHZ * 1000000,
            true,
            false,
            nullptr,
#endif
        },

        f4Timer_t{
#ifdef TIM10
            TIM10,
            TIM1_UP_TIM10_IRQn,
            {},
            f4Timer_t::clockSource_t::APB2,
            CORE_MCU_CPU_FREQ_MHZ * 1000000,
            true,
            false,
            nullptr,
#endif
        },

        f4Timer_t{
#ifdef TIM11
            TIM11,
            TIM1_TRG_COM_TIM11_IRQn,
            {},
            f4Timer_t::clockSource_t::APB2,
            CORE_MCU_CPU_FREQ_MHZ * 1000000,
            true,
            false,
            nullptr,
#endif
        },

        f4Timer_t{
#ifdef TIM13
            TIM13,
            TIM8_UP_TIM13_IRQn,
            {},
            f4Timer_t::clockSource_t::APB1,
            CORE_MCU_CPU_FREQ_MHZ * 1000000,
            true,
            false,
            nullptr,
#endif
        },
    };
}    // namespace

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
#ifdef TIM6
    if (htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
#endif

#ifdef TIM7
    if (htim->Instance == TIM7)
    {
        __HAL_RCC_TIM7_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
#endif

#ifdef TIM10
    if (htim->Instance == TIM10)
    {
        __HAL_RCC_TIM10_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    }
#endif

#ifdef TIM11
    if (htim->Instance == TIM11)
    {
        __HAL_RCC_TIM11_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    }
#endif

#ifdef TIM13
    if (htim->Instance == TIM13)
    {
        __HAL_RCC_TIM13_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    }
#endif
}

extern "C" void TIM6_DAC_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_timer[0].timer, TIM_IT_UPDATE);

    if (_timer[0].handler != nullptr)
    {
        _timer[0].handler();
    }
}

extern "C" void TIM7_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_timer[1].timer, TIM_IT_UPDATE);

    if (_timer[1].handler != nullptr)
    {
        _timer[1].handler();
    }
}

extern "C" void TIM1_UP_TIM10_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_timer[2].timer, TIM_IT_UPDATE);

    if (_timer[2].handler != nullptr)
    {
        _timer[2].handler();
    }
}

extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_timer[3].timer, TIM_IT_UPDATE);

    if (_timer[3].handler != nullptr)
    {
        _timer[3].handler();
    }
}

extern "C" void TIM8_UP_TIM13_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_timer[4].timer, TIM_IT_UPDATE);

    if (_timer[4].handler != nullptr)
    {
        _timer[4].handler();
    }
}

namespace core::mcu::timers
{
    bool init()
    {
        // nothing to do
        return true;
    }

    bool allocate(size_t& index, handler_t handler)
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
        {
            if (_timer[i].supported)
            {
                if (!_timer[i].allocated)
                {
                    _timer[i].timer.Instance               = _timer[i].instance;
                    _timer[i].timer.Init.Prescaler         = 0;
                    _timer[i].timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
                    _timer[i].timer.Init.Period            = 0;
                    _timer[i].timer.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
                    _timer[i].timer.Init.RepetitionCounter = 0;
                    _timer[i].timer.Init.AutoReloadPreload = 0;

                    HAL_TIM_Base_Init(&_timer[i].timer);

                    _timer[i].allocated = true;
                    index               = i;
                    _timer[i].handler   = handler;

                    RCC_ClkInitTypeDef clockConfig;
                    uint32_t           flashLatency;
                    HAL_RCC_GetClockConfig(&clockConfig, &flashLatency);
                    uint32_t divider = RCC_HCLK_DIV1;

                    switch (_timer[i].source)
                    {
                    case f4Timer_t::clockSource_t::APB1:
                    {
                        divider = clockConfig.APB1CLKDivider;
                    }
                    break;

                    case f4Timer_t::clockSource_t::APB2:
                    {
                        divider = clockConfig.APB2CLKDivider;
                    }
                    break;

                    default:
                        break;
                    }

                    // from the reference manual:
                    //  The timer clock frequencies are automatically set by hardware. There are two cases:
                    //  1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
                    //  that of the APB domain to which the timers are connected.
                    //  2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
                    //  timers are connected.
                    switch (divider)
                    {
                    case RCC_HCLK_DIV2:
                    {
                        _timer[i].clock /= 2;
                    }
                    break;

                    case RCC_HCLK_DIV4:
                    {
                        _timer[i].clock /= 4;
                    }
                    break;

                    case RCC_HCLK_DIV8:
                    {
                        _timer[i].clock /= 8;
                    }
                    break;

                    case RCC_HCLK_DIV16:
                    {
                        _timer[i].clock /= 16;
                    }
                    break;

                    default:
                        return true;
                    }

                    _timer[i].clock *= 2;

                    return true;
                }
            }
        }

        return false;
    }

    bool start(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        stop(index);
        __HAL_TIM_CLEAR_IT(&_timer[index].timer, TIM_IT_UPDATE);
        HAL_TIM_Base_Start_IT(&_timer[index].timer);

        return true;
    }

    void startAll()
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
        {
            start(i);
        }
    }

    bool stop(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        if (!isRunning(index))
        {
            return false;
        }

        HAL_TIM_Base_Stop_IT(&_timer[index].timer);

        if (HAL_NVIC_GetPendingIRQ(_timer[index].irqn))
        {
            HAL_NVIC_ClearPendingIRQ(_timer[index].irqn);
        }

        return true;
    }

    void stopAll()
    {
        for (size_t i = 0; i < NUM_OF_TIMERS; i++)
        {
            stop(i);
        }
    }

    bool isRunning(size_t index)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        return _timer[index].timer.Instance->CR1 & TIM_CR1_CEN;
    }

    bool setPeriod(size_t index, uint32_t us)
    {
        if (index >= _timer.size())
        {
            return false;
        }

        if (!_timer[index].allocated)
        {
            return false;
        }

        static uint32_t lastUs[NUM_OF_TIMERS] = {};
        uint32_t        prescaler             = 0;
        uint32_t        period                = 0;
        bool            match                 = false;

        if (lastUs[index] == us)
        {
            // nothing to do
            return true;
        }

        lastUs[index] = us;

        // convert microseconds to prescaler/period value
        float frequency = static_cast<float>(1) / (static_cast<float>(us) / static_cast<float>(1000000));

        for (prescaler = 1; prescaler < (0xFFFF + 1); prescaler++)
        {
            period = _timer[index].clock / (static_cast<uint32_t>(frequency) * prescaler);

            if (period <= 65536)
            {
                match = true;
                break;
            }
        }

        if (!match)
        {
            core::errorHandler(index);
        }
        else
        {
            _timer[index].timer.Init.Prescaler = prescaler - 1;
            _timer[index].timer.Init.Period    = period - 1;

            TIM_Base_SetConfig(_timer[index].timer.Instance, &_timer[index].timer.Init);
        }

        return match;
    }
}    // namespace core::mcu::timers