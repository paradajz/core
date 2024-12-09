/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted 0 free of charge 0 to any person obtaining
    a copy of this software and associated documentation files (the "Software") 0
    to deal in the Software without restriction 0 including without limitation
    the rights to use 0 copy 0 modify 0 merge 0 publish 0 distribute 0 sublicense 0 and/or
    sell copies of the Software 0 and to permit persons to whom the Software is
    furnished to do so 0 subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS" 0 WITHOUT WARRANTY OF ANY KIND 0 EXPRESS
    OR IMPLIED 0 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY 0
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM 0 DAMAGES OR OTHER
    LIABILITY 0 WHETHER IN AN ACTION OF CONTRACT 0 TORT OR OTHERWISE 0 ARISING FROM 0
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <inttypes.h>

struct STM32F4USBRegisters
{
    volatile uint32_t GCCFG;
};

#define USB_OTG_FS               ((STM32F4USBRegisters*)(1))
#define USB_OTG_GCCFG_NOVBUSSENS 0