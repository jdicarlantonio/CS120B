/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 4
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
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;    

    unsigned char upperNibble = 0x00;
    unsigned char lowerNibble = 0x00;
    
    while (1) 
    {
        upperNibble = PINA & 0xF0;
        lowerNibble = PINA & 0x0F;

        upperNibble >>= 4;
        lowerNibble <<= 4;
            
        PORTB = upperNibble;
        PORTC = lowerNibble;
    }
    return 1;
}
