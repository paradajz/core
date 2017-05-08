#include "UART.h"

///
/// \brief Stream used to send printf statements to UART.
///
static FILE uart_printf_stream;

///
/// \brief Buffer holding outgoing serial data.
/// See RingBuff_t enumeration.
///
RingBuff_t txBuffer;

///
/// \brief Buffer holding incoming serial data.
/// See RingBuff_t enumeration.
///
RingBuff_t rxBuffer;

///
/// \brief Default constructor (empty).
///
UART::UART()
{
    
}

///
/// \brief ISR used to store incoming data from UART to buffer.
///
ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;
    if (!RingBuffer_IsFull(&rxBuffer))
        RingBuffer_Insert(&rxBuffer, data);
}

///
/// \brief ISR used to send data from outgoing buffer to UART.
///
ISR(USART1_UDRE_vect)
{
    if (RingBuffer_IsEmpty(&txBuffer))
    {
        // buffer is empty, disable transmit interrupt
        UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
    }
    else
    {
        uint8_t data = RingBuffer_Remove(&txBuffer);
        UDR1 = data;
    }
}

///
/// \brief Empty ISR handler for UART transmission.
///
ISR(USART1_TX_vect)
{
    
}

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

///
/// \brief Initializes UART peripheral.
///
void UART::init()
{
    int16_t baud_count;

    //uart0
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    uart_printf_stream.put   = printChar;
    uart_printf_stream.get   = NULL;
    uart_printf_stream.flags = _FDEV_SETUP_WRITE;
    uart_printf_stream.udata = 0;
    stdout = &uart_printf_stream;

    baud_count = ((F_CPU / 8) + (BAUD_RATE_DEBUG / 2)) / BAUD_RATE_DEBUG;

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

    //uart1
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);

    baud_count = ((F_CPU / 8) + (BAUD_RATE_DISPLAY / 2)) / BAUD_RATE_DISPLAY;

    if ((baud_count & 1) && baud_count <= 4096)
    {
        //double speed uart
        UCSR1A = (1<<U2X1);
        UBRR1 = baud_count - 1;
    }
    else
    {
        UCSR1A = 0;
        UBRR1 = (baud_count >> 1) - 1;
    }

    //8 bit, no parity, 1 stop bit
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

    RingBuffer_InitBuffer(&rxBuffer);
    RingBuffer_InitBuffer(&txBuffer);
}

///
/// \brief Reads a byte from incoming UART buffer
/// \returns Single byte on success, -1 if buffer is empty.
///
int16_t UART::read()
{
    if (RingBuffer_IsEmpty(&rxBuffer))
        return -1;

    uint8_t data = RingBuffer_Remove(&rxBuffer);
    return data;
}

///
/// \brief Writes single byte to TX buffer.
/// @param [in] data    Byte value
/// \returns 0 on success, -1 otherwise.
///
int8_t UART::write(uint8_t data)
{
    if (!(UCSR1B & (1<<TXEN1)))
        return -1;

    if (RingBuffer_IsFull(&txBuffer))
        return -1;

    RingBuffer_Insert(&txBuffer, data);
    UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
    return 0;
}

///
/// \brief Checks if any incoming UART data is available.
/// \returns True if any data is available, false otherwise.
///
bool UART::available()
{
    return !RingBuffer_IsEmpty(&rxBuffer);
}

UART uart;
