/*
    Copyright 2017-2021 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <avr/io.h>

///
/// \brief Consistent UART register names for MCUs with single UART channel.
/// @{

#if defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)

#ifdef UCSR0A
#define UCSRA_0 UCSR0A
#elif defined UCSR1A
#define UCSRA_0 UCSR1A
#endif

#ifdef UCSR0B
#define UCSRB_0 UCSR0B
#elif defined UCSR1B
#define UCSRB_0 UCSR1B
#endif

#ifdef UCSR0C
#define UCSRC_0 UCSR0C
#elif defined UCSR1C
#define UCSRC_0 UCSR1C
#endif

#ifdef UDRE0
#define UDRE_0 UDRE0
#elif defined UDRE1
#define UDRE_0 UDRE1
#endif

#ifdef UDR0
#define UDR_0 UDR0
#elif defined UDR1
#define UDR_0 UDR1
#endif

#ifdef UDRIE0
#define UDRIE_0 UDRIE0
#elif defined UDRIE1
#define UDRIE_0 UDRIE1
#endif

#ifdef UBRR0
#define UBRR_0 UBRR0
#elif defined UBRR1
#define UBRR_0 UBRR1
#endif

#ifdef U2X0
#define U2X_0 U2X0
#elif defined U2X1
#define U2X_0 U2X1
#endif

#ifdef UCSZ00
#define UCSZ0_0 UCSZ00
#elif defined UCSZ10
#define UCSZ0_0 UCSZ10
#endif

#ifdef UCSZ01
#define UCSZ1_0 UCSZ01
#elif defined UCSZ11
#define UCSZ1_0 UCSZ11
#endif

#ifdef RXEN0
#define RXEN_0 RXEN0
#elif defined RXEN1
#define RXEN_0 RXEN1
#endif

#ifdef TXEN0
#define TXEN_0 TXEN0
#elif defined TXEN1
#define TXEN_0 TXEN1
#endif

#ifdef RXCIE0
#define RXCIE_0 RXCIE0
#elif defined RXCIE1
#define RXCIE_0 RXCIE1
#endif

#ifdef TXCIE0
#define TXCIE_0 TXCIE0
#elif defined TXCIE1
#define TXCIE_0 TXCIE1
#endif

#ifdef USART0_RX_vect
#define USART_RX_vect_0 USART0_RX_vect
#elif defined USART1_RX_vect
#define USART_RX_vect_0 USART1_RX_vect
#elif defined USART_RX_vect
#define USART_RX_vect_0 USART_RX_vect
#endif

#ifdef USART0_TX_vect
#define USART_TX_vect_0 USART0_TX_vect
#elif defined USART1_TX_vect
#define USART_TX_vect_0 USART1_TX_vect
#elif defined USART_TX_vect
#define USART_TX_vect_0 USART_TX_vect
#endif

#ifdef USART0_UDRE_vect
#define USART_UDRE_vect_0 USART0_UDRE_vect
#elif defined USART1_UDRE_vect
#define USART_UDRE_vect_0 USART1_UDRE_vect
#elif defined USART_UDRE_vect
#define USART_UDRE_vect_0 USART_UDRE_vect
#endif

#ifdef USBS0
#define USBS_0 USBS0
#elif defined USBS1
#define USBS_0 USBS1
#endif

#ifdef UPM00
#define UPM0_0 UPM00
#elif defined UPM10
#define UPM0_0 UPM10
#endif

#ifdef UPM01
#define UPM1_0 UPM01
#elif defined UPM11
#define UPM1_0 UPM11
#endif

#endif

/// @}

///
/// \brief Consistent UART register names for MCUs with multiple UART channels.
/// @{

#ifdef __AVR_ATmega2560__

#ifdef UCSR0A
#define UCSRA_0 UCSR0A
#endif

#if defined UCSR1A
#define UCSRA_1 UCSR1A
#endif

#if defined UCSR2A
#define UCSRA_2 UCSR2A
#endif

#if defined UCSR3A
#define UCSRA_3 UCSR3A
#endif

#ifdef UCSR0B
#define UCSRB_0 UCSR0B
#endif

#if defined UCSR1B
#define UCSRB_1 UCSR1B
#endif

#if defined UCSR2B
#define UCSRB_2 UCSR2B
#endif

#if defined UCSR3B
#define UCSRB_3 UCSR3B
#endif

#ifdef UCSR0C
#define UCSRC_0 UCSR0C
#endif

#if defined UCSR1C
#define UCSRC_1 UCSR1C
#endif

#if defined UCSR2C
#define UCSRC_2 UCSR2C
#endif

#if defined UCSR3C
#define UCSRC_3 UCSR3C
#endif

#ifdef UDRE0
#define UDRE_0 UDRE0
#endif

#if defined UDRE1
#define UDRE_1 UDRE1
#endif

#if defined UDRE2
#define UDRE_2 UDRE2
#endif

#if defined UDRE3
#define UDRE_3 UDRE3
#endif

#ifdef UDR0
#define UDR_0 UDR0
#endif

#if defined UDR1
#define UDR_1 UDR1
#endif

#if defined UDR2
#define UDR_2 UDR2
#endif

#if defined UDR3
#define UDR_3 UDR3
#endif

#ifdef UDRIE0
#define UDRIE_0 UDRIE0
#endif

#if defined UDRIE1
#define UDRIE_1 UDRIE1
#endif

#if defined UDRIE2
#define UDRIE_2 UDRIE2
#endif

#if defined UDRIE3
#define UDRIE_3 UDRIE3
#endif

#ifdef UBRR0
#define UBRR_0 UBRR0
#endif

#if defined UBRR1
#define UBRR_1 UBRR1
#endif

#if defined UBRR2
#define UBRR_2 UBRR2
#endif

#if defined UBRR3
#define UBRR_3 UBRR3
#endif

#ifdef U2X0
#define U2X_0 U2X0
#endif

#if defined U2X1
#define U2X_1 U2X1
#endif

#if defined U2X2
#define U2X_2 U2X2
#endif

#if defined U2X3
#define U2X_3 U2X3
#endif

#ifdef UCSZ00
#define UCSZ0_0 UCSZ00
#endif

#if defined UCSZ10
#define UCSZ0_1 UCSZ10
#endif

#if defined UCSZ20
#define UCSZ0_2 UCSZ20
#endif

#if defined UCSZ30
#define UCSZ0_3 UCSZ30
#endif

#ifdef UCSZ01
#define UCSZ1_0 UCSZ01
#endif

#if defined UCSZ11
#define UCSZ1_1 UCSZ11
#endif

#if defined UCSZ21
#define UCSZ1_2 UCSZ21
#endif

#if defined UCSZ31
#define UCSZ1_3 UCSZ31
#endif

#ifdef RXEN0
#define RXEN_0 RXEN0
#endif

#if defined RXEN1
#define RXEN_1 RXEN1
#endif

#if defined RXEN2
#define RXEN_2 RXEN2
#endif

#if defined RXEN3
#define RXEN_3 RXEN3
#endif

#ifdef TXEN0
#define TXEN_0 TXEN0
#endif

#if defined TXEN1
#define TXEN_1 TXEN1
#endif

#if defined TXEN2
#define TXEN_2 TXEN2
#endif

#if defined TXEN3
#define TXEN_3 TXEN3
#endif

#ifdef RXCIE0
#define RXCIE_0 RXCIE0
#endif

#if defined RXCIE1
#define RXCIE_1 RXCIE1
#endif

#if defined RXCIE2
#define RXCIE_2 RXCIE2
#endif

#if defined RXCIE3
#define RXCIE_3 RXCIE3
#endif

#ifdef TXCIE0
#define TXCIE_0 TXCIE0
#endif

#if defined TXCIE1
#define TXCIE_1 TXCIE1
#endif

#if defined TXCIE2
#define TXCIE_2 TXCIE2
#endif

#if defined TXCIE3
#define TXCIE_3 TXCIE3
#endif

#ifdef USART0_RX_vect
#define USART_RX_vect_0 USART0_RX_vect
#endif

#if defined USART1_RX_vect
#define USART_RX_vect_1 USART1_RX_vect
#endif

#if defined USART2_RX_vect
#define USART_RX_vect_2 USART2_RX_vect
#endif

#if defined USART3_RX_vect
#define USART_RX_vect_3 USART3_RX_vect
#endif

#ifdef USART0_TX_vect
#define USART_TX_vect_0 USART0_TX_vect
#endif

#if defined USART1_TX_vect
#define USART_TX_vect_1 USART1_TX_vect
#endif

#if defined USART2_TX_vect
#define USART_TX_vect_2 USART2_TX_vect
#endif

#if defined USART3_TX_vect
#define USART_TX_vect_3 USART3_TX_vect
#endif

#ifdef USART0_UDRE_vect
#define USART_UDRE_vect_0 USART0_UDRE_vect
#endif

#if defined USART1_UDRE_vect
#define USART_UDRE_vect_1 USART1_UDRE_vect
#endif

#if defined USART2_UDRE_vect
#define USART_UDRE_vect_2 USART2_UDRE_vect
#endif

#if defined USART3_UDRE_vect
#define USART_UDRE_vect_3 USART3_UDRE_vect
#endif

#ifdef USBS0
#define USBS_0 USBS0
#endif

#ifdef USBS1
#define USBS_1 USBS1
#endif

#ifdef USBS2
#define USBS_2 USBS2
#endif

#ifdef USBS3
#define USBS_3 USBS3
#endif

#ifdef UPM00
#define UPM0_0 UPM00
#endif

#ifdef UPM10
#define UPM0_1 UPM10
#endif

#ifdef UPM20
#define UPM0_2 UPM20
#endif

#ifdef UPM30
#define UPM0_3 UPM30
#endif

#ifdef UPM01
#define UPM1_0 UPM01
#endif

#ifdef UPM11
#define UPM1_1 UPM11
#endif

#ifdef UPM21
#define UPM1_2 UPM21
#endif

#ifdef UPM31
#define UPM1_3 UPM31
#endif

#endif

/// @}