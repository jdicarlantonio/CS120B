/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include "../header/time.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define LED PORTB

enum LEDStates
{
    START,
    LIGHT1,
    LIGHT2,
    LIGHT3
} state;

void ledSM()
{
    // transitions
    switch(state)
    {
        case START: state = LIGHT1; break;
        case LIGHT1: state = LIGHT2; break;
        case LIGHT2: state = LIGHT3; break;
        case LIGHT3: state = LIGHT1; break;
        default: state = LIGHT1; 
    }

    // actions
    switch(state)
    {
        case START:
        case LIGHT1: LED = 0x01; break;
        case LIGHT2: LED = 0x02; break;
        case LIGHT3: LED = 0x04; break;
        default: LED = 0x00; 
    }
}

int main(void) 
{
    DDRC = 0xFF; PORTC = 0x00;

    TimerSet(1000);
    TimerOn();

    state = START;
    while (1) 
    {
        ledSM();

        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
