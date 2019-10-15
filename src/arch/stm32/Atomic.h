/*
    Copyright 2017-2019 Igor Petrovic

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

#ifndef __CORE_STM32_ATOMIC
#define __CORE_STM32_ATOMIC

#include "cmsis_compiler.h"

static __inline__ uint8_t disableInterruptsRetVal(void)
{
    __disable_irq();
    return 1;
}

static __inline__ void restoreInterrupts(const uint32_t* __s)
{
    //if the interrupts have been enabled (PRIMASK returns 0), restore them
    if (!*__s)
        __enable_irq();
}

#define ATOMIC_SECTION                                                                            \
    for (uint32_t primask_save __attribute__((__cleanup__(restoreInterrupts))) = __get_PRIMASK(), \
                               condition = disableInterruptsRetVal();                             \
         condition;                                                                               \
         condition = 0)

#endif