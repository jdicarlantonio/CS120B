/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran 
 *	Lab Section: A21
 *	Assignment: Lab 3  Exercise 1
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
    unsigned char totalOnes = 0x00;
    unsigned char portsOnA = 0x00;
    unsigned char portsOnB = 0x00;
    while (1) 
    {
        totalOnes = 0x00;
        portsOnA = PINA;    
        portsOnB = PINB;
       
        unsigned char port = 0x01;
        for(unsigned char i = 0; i < 8; ++i)
        {
            if(portsOnA & port)
            {
                ++totalOnes; 
            }
            if(portsOnB & port)
            {
                ++totalOnes;
            }
            
            port <<= 1;
        }
     
        PORTC = totalOnes;
    }
    return 1;
}
