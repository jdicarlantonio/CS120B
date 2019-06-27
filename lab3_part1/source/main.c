/*	Author: jd
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
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;    
    DDRC = 0xFF; PORTC = 0x00;

    /* Insert your solution below */
    unsigned char portsOnA = 0x00;
    unsigned char portsOnB = 0x00;
    while (1) 
    {
        unsigned char portCount = 0x00;
        portsOnA = PINA;    
        portsOnB = PINB;

        for(unsigned char currPort = 0x01; currPort <= 0x80; currPort *= 2)
        {
            if(portsOnA & currPort)
            {
                ++portCount;
            }
            if(portsOnB & currPort)
            {
                ++portCount;
            }
        }
    }
    return 1;
}
