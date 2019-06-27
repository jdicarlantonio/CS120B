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

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    DDRC = 0xFF;
    
    unsigned char space = 0x00;
    unsigned char numOccupied = 0x00;
    while (1) 
    {
        space = PINA;
        numOccupied = 0x00;

        for(unsigned char currSpace = 0x01; currSpace <= 0x08; currSpace *= 0x02)
        {
            if(space & currSpace)
            {
                ++numOccupied;
            }
        }

        PORTC = 0x04 - numOccupied;
    }
   
    return 1;
}
