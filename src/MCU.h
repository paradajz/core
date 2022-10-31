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

#ifndef CORE_MCU_STUB
#ifdef CORE_ARCH_AVR
#include "arch/avr/atmel/common/MCU.h"
#elif defined(CORE_ARCH_ARM)
#ifdef CORE_VENDOR_ST
#include "arch/arm/st/common/MCU.h"
#elif defined(CORE_VENDOR_NORDIC)
#include "arch/arm/nordic/common/MCU.h"
#elif defined(CORE_VENDOR_RPF)
#include "arch/arm/rpf/common/MCU.h"
#else
#include "arch/stub/MCU.h"
#endif
#else
#include "arch/stub/MCU.h"
#endif
#else
#include "arch/stub/MCU.h"
#endif

#if __has_include(<MCU.h>)
#include <MCU.h>
#else
#error aaaaaaaa
#endif