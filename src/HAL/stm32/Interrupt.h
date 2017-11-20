#pragma once

#if defined(STM32) || defined(__DOXYGEN__)

#include "stm32f4xx_hal.h"

///
/// \defgroup corehalstmInt Interrupt
/// \ingroup coreHALstm
/// \brief Macros used to enable or disable interrupts globally.
/// @{

#define INT_DISABLE() \
__asm__ __volatile__ ( \
"cpsid i" : : : "memory" \
)

#define INT_ENABLE() \
__asm__ __volatile__ ( \
"cpsie i" : : : "memory" \
)

/// @}

#endif