#pragma once

#if defined(STM32) || defined(__DOXYGEN__)

#include "stm32f4xx_hal.h"

///
/// \brief Helper macros for easier port and pin manipulation.
/// These macros work only if CubeMX HAL is used.
/// \defgroup coreHALpinManAVR Pin manipulation
/// \ingroup coreHALstm
/// @{

#define setLow(port, pin)       (HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET))
#define setHigh(port, pin)      (HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET))
#define readPin(port, pin)      (HAL_GPIO_ReadPin(port, pin))

#endif