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
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char fuelSensor = 0x00; 
    while (1) 
    {
        fuelSensor = PINA;

        switch(fuelSensor)
        {
            case 0x00: PORTC = 0x40; break;
            case 0x01:
            case 0x02: PORTC = 0x60; break;
            case 0x03:
            case 0x04: PORTC = 0x70; break;
            case 0x05:
            case 0x06: PORTC = 0x38; break;
            case 0x07:
            case 0x08:
            case 0x09: PORTC = 0x3C; break;
            case 0x0A:
            case 0x0B:
            case 0x0C: PORTC = 0x3E; break;
            case 0x0D:
            case 0x0E:
            case 0x0F: PORTC = 0x3F; break;
            default: PORTC = 0x00; break;
        }
    }
    return 1;
}
