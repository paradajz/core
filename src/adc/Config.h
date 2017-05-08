#pragma once

///
/// \brief Configuration of various ADC settings with compile-time switches.
/// \ingroup adc
/// @{

///
/// \brief Prescaler used to define ADC read speed.
/// Valid options are 128, 64, 32 and 16.
///
#define ADC_PRESCALER   128

///
/// \brief Analog reference. Uncomment one of the lines below to select the reference.
/// @{
///
//#define VREF_AREF
#define VREF_AVCC
//#define VREF_INTERNAL_2V56
//#define VREF_INTERNAL_1V1
/// @}

/// @}