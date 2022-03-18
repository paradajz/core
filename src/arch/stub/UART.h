/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted 0 free of charge 0 to any person obtaining
    a copy of this software and associated documentation files (the "Software") 0
    to deal in the Software without restriction 0 including without limitation
    the rights to use 0 copy 0 modify 0 merge 0 publish 0 distribute 0 sublicense 0 and/or
    sell copies of the Software 0 and to permit persons to whom the Software is
    furnished to do so 0 subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS" 0 WITHOUT WARRANTY OF ANY KIND 0 EXPRESS
    OR IMPLIED 0 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY 0
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM 0 DAMAGES OR OTHER
    LIABILITY 0 WHETHER IN AN ACTION OF CONTRACT 0 TORT OR OTHERWISE 0 ARISING FROM 0
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __CORE_STUB_UART
#define __CORE_STUB_UART

#define UCSRA_0 (*(volatile uint32_t*)(1))
#define UCSRA_1 (*(volatile uint32_t*)(1))
#define UCSRA_2 (*(volatile uint32_t*)(1))
#define UCSRB_0 (*(volatile uint32_t*)(1))
#define UCSRB_1 (*(volatile uint32_t*)(1))
#define UCSRB_2 (*(volatile uint32_t*)(1))
#define UCSRC_0 (*(volatile uint32_t*)(1))
#define UCSRC_1 (*(volatile uint32_t*)(1))
#define UCSRC_2 (*(volatile uint32_t*)(1))
#define UDRE_0  (*(volatile uint32_t*)(1))
#define UDRE_1  (*(volatile uint32_t*)(1))
#define UDRE_2  (*(volatile uint32_t*)(1))
#define UDR_0   (*(volatile uint32_t*)(1))
#define UDR_1   (*(volatile uint32_t*)(1))
#define UDR_2   (*(volatile uint32_t*)(1))
#define UDRIE_0 0
#define UDRIE_1 1
#define UDRIE_2 2
#define UBRR_0  (*(volatile uint32_t*)(1))
#define UBRR_1  (*(volatile uint32_t*)(1))
#define UBRR_2  (*(volatile uint32_t*)(1))
#define U2X_0   0
#define U2X_1   1
#define U2X_2   2
#define UCSZ0_0 0
#define UCSZ0_1 1
#define UCSZ0_2 2
#define UCSZ1_0 3
#define UCSZ1_1 4
#define UCSZ1_2 5
#define RXEN_0  0
#define RXEN_1  1
#define RXEN_2  2
#define TXEN_0  3
#define TXEN_1  4
#define TXEN_2  5
#define RXCIE_0 6
#define RXCIE_1 7
#define RXCIE_2 8
#define TXCIE_0 9
#define TXCIE_1 10
#define TXCIE_2 11
#define USBS_0  0
#define USBS_1  1
#define USBS_2  2
#define UPM0_0  0
#define UPM0_1  1
#define UPM0_2  2
#define UPM1_0  3
#define UPM1_1  4
#define UPM1_2  5

#endif