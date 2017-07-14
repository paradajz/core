#if defined(USE_RX) || defined(USE_TX)

#include "UART.h"
#include "RingBuffer/RingBuffer.h"
#include <avr/interrupt.h>

#ifdef USE_TX
///
/// \brief Buffer in which outgoing data is stored.
///
RingBuff_t  txBuffer;
#endif

#ifdef USE_RX
///
/// \brief Buffer in which incoming data is stored.
///
RingBuff_t  rxBuffer;
#endif

#ifdef USE_TX_DEBUG
///
/// \brief Stream used to send printf statements to UART.
///
static FILE uart_printf_stream;

///
/// \brief Prints a character to FILE stream
/// @param [in] c   Character to write
///
int printChar(char c, FILE *stream)
{
    //write directly to UART
    while ((UCSR0A & (1 << UDRE0)) == 0) {};
    UDR0 = c;
    return 0;
}
#endif

///
/// \brief Default constructor.
///
UART::UART()
{
    
}

#ifdef USE_RX
///
/// \brief ISR used to store incoming data from UART to buffer.
///
ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;

    if (!RingBuffer_IsFull(&rxBuffer))
        RingBuffer_Insert(&rxBuffer, data);
}
#endif

#ifdef USE_TX
///
/// \brief Empty ISR handler for UART transmission.
///
ISR(USART1_TX_vect)
{
    
}
#endif

#ifdef USE_TX
///
/// \brief ISR used to send data from outgoing buffer to UART.
///
ISR(USART1_UDRE_vect)
{
    if (RingBuffer_IsEmpty(&txBuffer))
    {
        //buffer is empty, disable transmit interrupt
        #ifdef USE_RX
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
/// @param [in] baudRate UART baudrate.
///
void UART::init
(uint32_t baudRate
#ifdef USE_TX_DEBUG
, uint32_t baudRate_debug
#endif
)
{
    int16_t baud_count;

    #ifdef USE_TX_DEBUG
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    uart_printf_stream.put   = printChar;
    uart_printf_stream.get   = NULL;
    uart_printf_stream.flags = _FDEV_SETUP_WRITE;
    uart_printf_stream.udata = 0;
    stdout = &uart_printf_stream;

    baud_count = ((F_CPU / 8) + (baudRate_debug / 2)) / baudRate_debug;

    if ((baud_count & 1) && baud_count <= 4096)
    {
        //double speed uart
        UCSR0A = (1<<U2X0);
        UBRR0 = baud_count - 1;
    }
    else
    {
        UCSR0A = 0;
        UBRR0 = (baud_count >> 1) - 1;
    }

    //8 bit, no parity, 1 stop bit
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
    #endif

    baud_count = ((F_CPU / 8) + (baudRate / 2)) / baudRate;

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

    #if (defined(USE_RX) && defined(USE_TX))
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
    #endif

    #if (defined(USE_RX) && !defined(USE_TX))
    UCSR1B = (1<<RXEN1) | (1<<RXCIE1);
    #endif

    #if (!defined(USE_RX) && defined(USE_TX))
    UCSR1B = (1<<TXCIE1) | (1<<TXEN1);
    #endif

    //8 bit, no parity, 1 stop bit
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

    #ifdef USE_RX
    RingBuffer_InitBuffer(&rxBuffer);
    #endif

    #ifdef USE_TX
    RingBuffer_InitBuffer(&txBuffer);
    #endif
}

#ifdef USE_TX
///
/// \brief Writes single byte to TX buffer.
/// @param [in] data    Byte value
/// \returns 0 on success, -1 otherwise.
///
int8_t UART::write(uint8_t data)
{
    #if !defined(USE_TX)
    return -1;
    #endif

    if (RingBuffer_IsFull(&txBuffer))
        return -1;

    RingBuffer_Insert(&txBuffer, data);

    #ifdef USE_RX
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
    #else
    UCSR1B = (1<<TXCIE1) | (1<<TXEN1) | (1<<UDRIE1);
    #endif

    return 0;
}
#endif

///
/// \brief Checks if any incoming UART data is available.
/// \returns True if any data is available, false otherwise.
///
bool UART::rxAvailable()
{
    #if !defined(USE_RX)
    return 0;
    #else
    return !RingBuffer_IsEmpty(&rxBuffer);
    #endif
}

///
/// \brief Reads a byte from incoming UART buffer.
/// \returns Single byte on success, -1 is buffer is empty.
///
int16_t UART::read()
{
    #if !defined(USE_RX)
    return -1;
    #else
    if (RingBuffer_IsEmpty(&rxBuffer))
        return -1;

    uint8_t data = RingBuffer_Remove(&rxBuffer);
    return data;
    #endif
}

UART uart;

#endif