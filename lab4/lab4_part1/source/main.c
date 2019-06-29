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

#define button (PINA & 0x01)

enum LightStates 
{
    LED0,
    WAIT1,
    LED1,
    WAIT0
} state;

void tick()
{
    // switch for transitions
    switch(state)
    {
        case LED0:
        {
            state = button ? WAIT1 : LED0;
            break;
        }
        case WAIT1:
        {
            state = button ? WAIT1: LED1;
            break;
        }
        case LED1:
        {
            state = button ? WAIT0 : LED1;
            break;
        }
        case WAIT0:
        {
            state = button ? WAIT0 : LED0;
            break;
        }
    }

    // switch for actions
    switch(state)
    {
        case LED1:
        case WAIT1: PORTB = 0x02; break;
        case LED0:
        case WAIT0: 
        default: PORTB = 0x01; break; 
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;    

    state = LED0;
    while (1) 
    {
        tick();    
    }
    return 1;
}
