#pragma once

#if defined(__ARCH_AVR__) || defined(__DOXYGEN__)
#include <avr/interrupt.h>

///
/// \defgroup corehalavrInt Interrupt
/// \ingroup coreHALavr
/// \brief Macros used to enable or disable interrupts globally.
/// @{

#define INT_DISABLE()   cli()
#define INT_ENABLE()    sei()

/// @}
#endif