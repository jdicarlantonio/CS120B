/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    unsigned char led = 0x00;
    unsigned char button = 0x00;
    while (1) 
    {
        button = ~PINA & 0x01;

        if(button)
        {
            led = (led & 0xFC) | 0x01;
        }
        else
        {
            led = (led & 0xFC) | 0x02;
        }

        PORTB = led;
    }
    return 1;
}
