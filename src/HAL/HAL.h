#pragma once

#ifdef __ARCH_AVR__
#include "avr/AVR.h"
#elif defined(STM32)
#include "stm32/STM32.h"
#else
//make sure module can be compiled despite lack of HAL
#define INT_ENABLE()
#define INT_DISABLE()
#endif