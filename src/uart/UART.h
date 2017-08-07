#pragma once

///
/// \ingroup uart
/// @{

#if defined(USE_RX) || defined(USE_TX) || defined(__DOXYGEN__)

class UART
{
    public:
    UART();
    void init
    (
    uint32_t baudRate
    #ifdef USE_TX_DEBUG
    , uint32_t baudRate_debug
    #endif
    );
    bool rxAvailable();
    int16_t read();
    int8_t write(uint8_t data);
};

extern UART uart;

/// @}

#endif