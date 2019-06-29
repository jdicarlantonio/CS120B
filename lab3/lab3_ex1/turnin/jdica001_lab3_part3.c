/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran 
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 3
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
    unsigned char keySensor = 0x00;
    unsigned char driverSensor = 0x00;
    unsigned char beltSensor = 0x00;
    unsigned char fuelAmount = 0x00;
    while (1) 
    {
        fuelSensor = PINA & 0x0F;
        keySensor = PINA & 0x10;
        driverSensor = PINA & 0x20;
        beltSensor = PINA & 0x40;

        switch(fuelSensor)
        {
            case 0x00: fuelAmount = 0x40; break;
            case 0x01:
            case 0x02: fuelAmount = 0x60; break;
            case 0x03:
            case 0x04: fuelAmount = 0x70; break;
            case 0x05:
            case 0x06: fuelAmount = 0x38; break;
            case 0x07:
            case 0x08:
            case 0x09: fuelAmount = 0x3C; break;
            case 0x0A:
            case 0x0B:
            case 0x0C: fuelAmount = 0x3E; break;
            case 0x0D:
            case 0x0E:
            case 0x0F: fuelAmount = 0x3F; break;
        }

        if(keySensor && driverSensor && !beltSensor)
        {
            PORTC = fuelAmount | 0x80; 
        }
        else
        {
            PORTC = fuelAmount;
        }
    }
    return 1;
}
