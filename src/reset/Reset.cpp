#include "Reset.h"

#define WDFR 3

void disablePeripherals(void)
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
    TIMSK0 = 0;
    TIMSK1 = 0;
    TIMSK2 = 0;
    TIMSK3 = 0;
    TIMSK4 = 0;
    TIMSK5 = 0;

    //disable USART
    UCSR0B = 0;
    UCSR1B = 0;
    UCSR2B = 0;
    UCSR3B = 0;

    //disable I2C
    TWCR = 0;

    //write low to all pins
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTF = 0;
    PORTG = 0;
    PORTH = 0;
    PORTJ = 0;
    PORTK = 0;
    PORTL = 0;

    //set all pins to inputs
    DDRA = 0;
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    DDRE = 0;
    DDRF = 0;
    DDRG = 0;
    DDRH = 0;
    DDRJ = 0;
    DDRK = 0;
    DDRL = 0;
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