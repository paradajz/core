#include "Core.h"

void coreInit()
{
    cli();

    //init adc peripheral
    setUpADC();

    //init uart
    uart.init();

    sei();
}