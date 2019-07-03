/*	Author: Joseph DiCarlantonio
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

#define BUTTON (PINA & 0x01)

#define MAX_LIGHTS 14 

const unsigned char pattern[MAX_LIGHTS] = {
    0x00, 0x01, 0x02, 
    0x04, 0x08, 0x10,
    0x20, 0x00, 0x20,
    0x10, 0x08, 0x04,
    0x02, 0x01
};

unsigned char count = 0x00;

enum States
{
    INIT,
    WAIT,
    BUTTON_PRESSED
} state;

void tick()
{
    // transitions
    switch(state)
    {
        case INIT: state = WAIT; break;
        case WAIT:
        {
            if(BUTTON && (count + 1) < MAX_LIGHTS) 
            {
                ++count;
                state = BUTTON_PRESSED;
            }
            else if(BUTTON && (count + 1) >= MAX_LIGHTS)
            {
                count = 0x00;
                state = BUTTON_PRESSED;
            }
            else
            {
                state = WAIT; 
            }

            break;
        }
        case BUTTON_PRESSED:
        {
            if(BUTTON) 
            {
                state = BUTTON_PRESSED;
            }
            else
            {
                state = WAIT; 
            }

            break;
        }
    }

    unsigned char led = 0x00;
    // actions
    switch(state)
    {
        case INIT:
        {
            count = 0x00;
            led = pattern[count];

            break;
        }
        case WAIT:
        {
            led = pattern[count];

            break;
        }
        case BUTTON_PRESSED:
        {
            led = pattern[count];

            break;
        }
    }

    PORTC = led;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    while(1) 
    {
        tick();
    }
    return 1;
}
