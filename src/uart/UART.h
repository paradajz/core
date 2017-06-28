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
    void begin
	(
	uint32_t baudRate
	#ifdef USE_TX_DEBUG
	, uint32_t baudRate_debug
	#endif
	);
    bool available();
    int16_t read();
    int8_t write(uint8_t data);
};

extern UART uart;
/// @}