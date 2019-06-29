/*	Author: jd
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

#define MAX_WEIGHT 0x46
#define MIN_WEIGHT 0x05

int main(void) 
{
    /* Insert DDR and PORT initializations */
    DDRB = 0xFE; PORTB = 0x01;
    DDRD = 0x00; PORTD = 0xFF; 
    
    unsigned short weightSensor = 0x00;
    while (1) 
    {
        weightSensor = (PIND << 0x01) | (PINB & 0x01);

        if(weightSensor >= MAX_WEIGHT) 
        {
            PORTB = 0x02;
        }
        else if(weightSensor > MIN_WEIGHT && weightSensor < MAX_WEIGHT)
        {
            PORTB = 0x04;
        }
        else
        {
            PORTB = 0x00;
        }
    }
    
    return 1;
}
