#include "UART.h"
#include "RingBuffer/RingBuffer.h"
#include <avr/interrupt.h>

#if USE_TX > 0
RingBuff_t  txBuffer;
#endif
#if USE_RX > 0
RingBuff_t  rxBuffer;
#endif

///
/// \brief Default constructor.
///
UART::UART()
{
    
}

#if USE_RX > 0
///
/// \brief ISR used to store incoming data from UART to buffer.
///
ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;

    if (!RingBuffer_IsFull(&rxBuffer))
        RingBuffer_Insert(&rxBuffer, data);
}

ISR(USART1_TX_vect)
{
    
}
#endif

#if USE_TX > 0
///
/// \brief ISR used to send data from outgoing buffer to UART.
///
ISR(USART1_UDRE_vect)
{
    if (RingBuffer_IsEmpty(&txBuffer))
    {
        //buffer is empty, disable transmit interrupt
        #if USE_RX > 0
        UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
        #else
        UCSR1B = (1<<TXCIE1) | (1<<TXEN1);
        #endif
    }
    else
    {
        uint8_t data = RingBuffer_Remove(&txBuffer);
        UDR1 = data;
    }
}
#endif

///
/// \brief Initializes UART peripheral.
///
void UART::begin(uint32_t baudRate)
{
    int16_t baud_count = ((F_CPU / 8) + (baudRate / 2)) / baudRate;

    if ((baud_count & 1) && baud_count <= 4096)
    {
        UCSR1A = (1<<U2X1); //double speed uart
        UBRR1 = baud_count - 1;
    }
    else
    {
        UCSR1A = 0;
        UBRR1 = (baud_count >> 1) - 1;
    }

    #if ((USE_RX > 0) && (USE_TX > 0))
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
    #endif

    #if ((USE_RX > 0) && (USE_TX == 0))
    UCSR1B = (1<<RXEN1) | (1<<RXCIE1);
    #endif

    #if ((USE_RX == 0) && (USE_TX > 0))
    UCSR1B = (1<<TXCIE1) | (1<<TXEN1);
    #endif

    //8 bit, no parity, 1 stop bit
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

    #if USE_RX > 0
    RingBuffer_InitBuffer(&rxBuffer);
    #endif

    #if USE_TX > 0
    RingBuffer_InitBuffer(&txBuffer);
    #endif
}

///
/// \brief Writes single byte to TX buffer.
/// @param [in] data    Byte value
/// \returns 0 on success, -1 otherwise.
///
int8_t UART::write(uint8_t data)
{
    #if USE_TX == 0
        return -1;
    #endif

    if (RingBuffer_IsFull(&txBuffer))
        return -1;

    RingBuffer_Insert(&txBuffer, data);

    #if USE_RX > 0
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
    #else
    UCSR1B = (1<<TXCIE1) | (1<<TXEN1) | (1<<UDRIE1);
    #endif

    return 0;
}

///
/// \brief Checks if any incoming UART data is available.
/// \returns True if any data is available, false otherwise.
///
bool UART::available()
{
    #if USE_RX == 0
    return 0;
    #else
    return !RingBuffer_IsEmpty(&rxBuffer);
    #endif
}

///
/// \brief Reads a byte from incoming UART buffer
/// \returns Single byte on success, -1 is buffer is empty.
///
int16_t UART::read()
{
    #if USE_RX == 0
    return -1;
    #else
    if (RingBuffer_IsEmpty(&rxBuffer))
        return -1;

    uint8_t data = RingBuffer_Remove(&rxBuffer);
    return data;
    #endif
}

UART uart;
