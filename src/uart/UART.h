#pragma once

///
/// \brief Object for interfacing with configured UART peripheral.
/// \defgroup uart UART
/// \ingroup core
/// @{

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