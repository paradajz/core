/*
    Copyright 2017-2022 Igor Petrovic

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

#pragma once

#ifndef CORE_MCU_GENERATED
#error This file requires generated MCU header
#endif

#include <avr/boot.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/io.h>
#include "Interrupt.h"
#include "IO.h"
#include "Peripherals.h"
#include "SPI.h"
#include "UART.h"
#include "core/src/arch/avr/common/Atomic.h"
#include "core/src/arch/common/ADC.h"
#include "core/src/arch/common/Bootloader.h"
#include "core/src/arch/common/I2C.h"
#include "core/src/arch/common/Flash.h"
#include "core/src/arch/common/ISR.h"
#include "core/src/arch/common/Timers.h"
#include <MCU.h>
#include "core/src/arch/common/usb/USB.h"
#include "core/src/arch/common/MCU.h"

namespace core::mcu
{
    inline void init(initType_t initType = initType_t::APP)
    {
        CORE_MCU_DISABLE_INTERRUPTS();

        if (initType == initType_t::BOOT)
        {
            // relocate the interrupt vector table to the bootloader section
            MCUCR = (1 << IVCE);
            MCUCR = (1 << IVSEL);
        }
        else
        {
            // relocate the interrupt vector table to the application section
            MCUCR = (1 << IVCE);
            MCUCR = (0 << IVSEL);
        }

        // clear reset source
        MCUSR &= ~(1 << EXTRF);

        // disable watchdog
        MCUSR &= ~(1 << WDRF);
        wdt_disable();

        // no point in putting this in a dedicated clocks::init function (also saves flash!)
        clock_prescale_set(clock_div_1);

        CORE_MCU_ENABLE_INTERRUPTS();
    }

    inline void deInit()
    {
        // disable eeprom
        EECR = 0;

        // disable analog comparator
        ACSR = 0;

        // disable SPI
        SPCR = 0;

        // disable external interrupts
        EIMSK = 0;

        // disable pin change interrupts
        PCICR = 0;

// disable ADC
#ifdef ADCSRA
        ADCSRA = 0;
#endif

// disable timers
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

// disable USART
#ifdef UCSR0A
        UCSR0A = 0;
#endif
#ifdef UCSR1A
        UCSR1A = 0;
#endif
#ifdef UCSR2A
        UCSR2A = 0;
#endif
#ifdef UCSR3A
        UCSR3A = 0;
#endif

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

#ifdef UCSR0C
        UCSR0C = 0;
#endif
#ifdef UCSR1C
        UCSR1C = 0;
#endif
#ifdef UCSR2C
        UCSR2C = 0;
#endif
#ifdef UCSR3C
        UCSR3C = 0;
#endif

// disable I2C
#ifdef TWCR
        TWCR = 0;
        TWSR = 0;
        TWCR = 0;
#endif

// write low to all pins
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

// set all pins to inputs
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

#ifdef USBCON
        // detach USB connection
        UDCON |= (1 << DETACH);
#endif
    }

    /// Initiates software MCU reset by setting watch-dog timeout and waiting until watchdog is activated.
    inline void reset()
    {
        cli();
        // stop watchdog timer, if running

#ifndef WDFR
        static constexpr uint8_t WDFR = 3;
#endif

        MCUSR &= ~(1 << WDFR);
        WDTCSR |= (1 << WDCE);
        WDTCSR = 0;
        _delay_ms(5);
        deInit();
        wdt_enable(WDTO_15MS);

        while (1)
        {
            ;
        }
    }

    inline void idle()
    {
    }
}    // namespace core::mcu

#if defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)
// serial numbers are available only on AVR MCUs with USB

namespace core::mcu
{
    inline void uniqueID(uniqueID_t& uid)
    {
        CORE_MCU_ATOMIC_SECTION
        {
            uint8_t address = 0x0E;

            for (uint8_t i = 0; i < (CORE_MCU_UID_BITS / 8); i++)
            {
                uid[i] = boot_signature_byte_get(address++);

                // LUFA sends unique ID with nibbles swaped
                // to match with LUFA, invert them here
                uid[i] = (uid[i] << 4) | ((uid[i] >> 4) & 0x0F);
            }
        }
    }
}    // namespace core::mcu

#endif
