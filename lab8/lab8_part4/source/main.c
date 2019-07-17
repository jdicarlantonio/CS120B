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

#define MAX 800
#define MAX_TWO 700
#define MAX_THREE 600
#define MAX_FOUR 500
#define MAX_FIVE 400
#define MAX_SIX 300
#define MAX_SEVEN 200
#define MAX_EIGHT 100

void ADC_init()
{
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;
//    DDRD = 0xFF; PORTD = 0x00;
//    DDRA = 0x00; PORTA = 0xFF; 

    ADC_init();
    while(1) 
    {
        unsigned short voltage = ADC;

        if(voltage >= MAX)
        {
            PORTB = 0xFF;  
        }
        else if((voltage >= MAX_TWO))
        {
            PORTB = 0x7F; 
        }
        else if((voltage >= MAX_THREE))
        {
            PORTB = 0x3F; 
        }
        else if((voltage >= MAX_FOUR))
        {
            PORTB = 0x1F; 
        }
        else if((voltage >= MAX_FIVE))
        {
            PORTB = 0x0F; 
        }
        else if((voltage >= MAX_SIX))
        {
            PORTB = 0x07; 
        }
        else if((voltage >= MAX_SEVEN))
        {
            PORTB = 0x03; 
        }
        else if((voltage >= MAX_EIGHT))
        {
            PORTB = 0x01; 
        }
    }

    return 1;
}
