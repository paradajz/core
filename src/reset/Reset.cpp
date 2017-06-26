#include "Reset.h"

#define WDFR 3

void disablePeripherals()
{
    //disable eeprom
    EECR = 0;

    //disable analog comparator
    ACSR = 0;

    //disable SPI
    SPCR = 0;

    //disable external interrupts
    EIMSK = 0;

    //disable pin change interrupts
    PCICR = 0;

    //disable ADC
    ADCSRA = 0;

    //disable timers
    #ifdef TIMSK0
    TIMSK0 = 0;
    #endif
    #ifdef TIMSK1
    TIMSK1 = 0;
    #endif
    #ifdef TIMSK2
    TIMSK2 = 0;
    #endif
    #ifdef TIMSK3
    TIMSK3 = 0;
    #endif
    #ifdef TIMSK4
    TIMSK4 = 0;
    #endif
    #ifdef TIMSK5
    TIMSK5 = 0;
    #endif

    //disable USART
    #ifdef UCSR0B
    UCSR0B = 0;
    #endif
    #ifdef UCSR1B
    UCSR1B = 0;
    #endif
    #ifdef UCSR2B
    UCSR2B = 0;
    #endif
    #ifdef UCSR3B
    UCSR3B = 0;
    #endif

    //disable I2C
    TWCR = 0;

    //write low to all pins
    #ifdef PORTA
    PORTA = 0;
    #endif
    #ifdef PORTB
    PORTB = 0;
    #endif
    #ifdef PORTC
    PORTC = 0;
    #endif
    #ifdef PORTD
    PORTD = 0;
    #endif
    #ifdef PORTE
    PORTE = 0;
    #endif
    #ifdef PORTF
    PORTF = 0;
    #endif
    #ifdef PORTG
    PORTG = 0;
    #endif
    #ifdef PORTH
    PORTH = 0;
    #endif
    #ifdef PORTJ
    PORTJ = 0;
    #endif
    #ifdef PORTK
    PORTK = 0;
    #endif
    #ifdef PORTL
    PORTL = 0;
    #endif

    //set all pins to inputs
    #ifdef DDRA
    DDRA = 0;
    #endif
    #ifdef DDRB
    DDRB = 0;
    #endif
    #ifdef DDRC
    DDRC = 0;
    #endif
    #ifdef DDRD
    DDRD = 0;
    #endif
    #ifdef DDRE
    DDRE = 0;
    #endif
    #ifdef DDRF
    DDRF = 0;
    #endif
    #ifdef DDRG
    DDRG = 0;
    #endif
    #ifdef DDRH
    DDRH = 0;
    #endif
    #ifdef DDRJ
    DDRJ = 0;
    #endif
    #ifdef DDRK
    DDRK = 0;
    #endif
    #ifdef DDRL
    DDRL = 0;
    #endif
}

void mcuReset()
{
    cli();
    //stop watchdog timer, if running
    MCUSR &= ~(1<<WDFR);
    WDTCSR |= (1<<WDCE);
    WDTCSR = 0;
    _delay_ms(5);
    disablePeripherals();

    wdt_enable(WDTO_15MS);
    while(1);
}

void wdt_init(void)
{
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    return;
}