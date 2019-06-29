/*	Author: jdica001
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
    DDRB = 0xFF;
    /* Insert your solution below */

    unsigned char doorSensor = 0x00;
    unsigned char lightSensor = 0x00;
    unsigned char ledOut = 0x00;
    while (1)
    { 
        doorSensor = PINA & 0x01;
        lightSensor = PINA & 0x02;
        if(doorSensor && !lightSensor)
        {
            ledOut = 0x01;
        }
        else
        {
            ledOut = 0x00;
        }

        PORTB = ledOut;
    }
    return 1;
}
