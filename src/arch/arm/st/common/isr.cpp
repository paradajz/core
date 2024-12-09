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

#include "core/arch/arm/st/common/hal.h"

// STM32 common ISRs

// This function handles Non maskable interrupt.
extern "C" void NMI_Handler(void)
{
}

// This function handles Memory management fault.
extern "C" void MemManage_Handler(void)
{
    /* USER CODE END MemoryManagement_IRQn 0 */
    while (true)
    {
    }
}

// This function handles Pre-fetch fault, memory access fault.
extern "C" void BusFault_Handler(void)
{
    while (true)
    {
    }
}

// This function handles Undefined instruction or illegal state.
extern "C" void UsageFault_Handler(void)
{
    while (true)
    {
    }
}

// This function handles System service call via SWI instruction.
extern "C" void SVC_Handler(void)
{
}

// This function handles Debug monitor.
extern "C" void DebugMon_Handler(void)
{
}

// This function handles Pendable request for system service.
extern "C" void PendSV_Handler(void)
{
}

// This function handles System tick timer.
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}