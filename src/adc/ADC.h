#pragma once

#if defined(USE_ADC) || defined(__DOXYGEN__)

///
/// \ingroup adc
/// @{

///
/// \brief Starts ADC conversion by setting ADSC bit in ADCSRA register.
///
#define startADCconversion() (ADCSRA |= (1<<ADSC))

///
/// \brief Enables ADC interrupts by setting ADIE bit in ADCSRA register.
///
#define adcInterruptEnable() (ADCSRA |= (1<<ADIE))

///
/// \brief Sets up ADC according to parameters specified in Config.h
///
void setUpADC();

///
/// \brief Get ADC value from last set ADC channel.
/// \return Value from ADC registers (ADCH and ADCL).
///
inline uint16_t getADCvalue()
{
    //single conversion mode
    ADCSRA |= (1<<ADSC);

    //wait until ADC conversion is complete
    while (ADCSRA & (1<<ADSC));

    return ADC;
}

///
/// \brief Disable digital input circuitry on specified ADC channel to reduce noise.
/// @param[in] adcChannel ADC Channel.
///
inline void disconnectDigitalInADC(uint8_t adcChannel)
{
    if (adcChannel < 6)
        DIDR0 |= (1<<adcChannel);
}

///
/// \brief Sets active ADC channel
/// @param[in] adcChannel ADC Channel.
///
inline void setADCchannel(uint8_t adcChannel)
{
    //check for valid channel
    if ((adcChannel < 0) || (adcChannel > 7))
        return;

    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (adcChannel & 0x0F);
}

/// @}

//#endif