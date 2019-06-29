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

#define X       (PINA & 0x01)
#define Y       (PINA & 0x02)
#define sharp   (PINA & 0x04)
#define inHouse (PINA & 0x80)

enum States
{
    WAIT,
    SHARP_DOWN,
    SHARP_RELEASE,
    Y_DOWN,
    UNLOCK,
    INHOUSE_DOWN,
    LOCK
} state;

void tick()
{
    // transitions
    switch(state)
    {
        case WAIT:
        {
            if((Y || X) && !sharp)
            {
                state = WAIT;
            }
            else if(sharp && !Y && !X)
            {
                state = SHARP_DOWN;
            }
            else if(inHouse)
            {
                state = INHOUSE_DOWN;
            }

            break;
        }
        case SHARP_DOWN:
        {
            if(sharp && (Y || X))
            {
                state = WAIT;
            }
            else if(sharp)
            {
                state = SHARP_DOWN;
            }
            else if(!sharp)
            {
                state = SHARP_RELEASE;
            }
           
            break; 
        }
        case SHARP_RELEASE:
        {
            if(Y && !sharp && !X)
            {
                state = Y_DOWN;
            }
            else if(sharp || X)
            {
                state = WAIT;
            }
        }
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    state = WAIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
