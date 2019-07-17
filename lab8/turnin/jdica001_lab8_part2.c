/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
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

void ADC_init()
{
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
//    DDRA = 0x00; PORTA = 0xFF;
    
    ADC_init();
    while (1) 
    {
        unsigned short voltage = ADC;
        unsigned char first = (char)voltage;
        unsigned char last = (char)(voltage >> 8);

        PORTB = first;
        PORTD = last;
    }

    return 1;
}
