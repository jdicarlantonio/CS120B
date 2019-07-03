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

#define X       (PINA & 0x01)
#define Y       (PINA & 0x02)
#define sharp   (PINA & 0x04)
#define inHouse (PINA & 0x80)

enum DoorLock
{
    LOCKED = 0,
    UNLOCKED
} door;

enum States
{
    WAIT,               // 0
    SHARP_DOWN,         // 1
    SHARP_RELEASE,      // 2
    Y_DOWN,             // 3
    UNLOCK,             // 4
} state;

void tick()
{
    // transitions
    switch(state)
    {
        case WAIT:
        {
            if(sharp && !X && !Y) 
            {
                state = SHARP_DOWN; 
            }
            else
            {
                state = WAIT; 
            }

            break;
        }
        case SHARP_DOWN:
        {
            if(!sharp && !X && !Y) 
            {
                state = SHARP_RELEASE; 
            }
            else if(sharp && (X || Y))
            {
                state = WAIT; 
            }
            else if(sharp && !X && !Y)
            {
                state = SHARP_DOWN; 
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
            else if(!sharp && !X && !Y)
            {
                state = SHARP_RELEASE; 
            }

            break;
        }
        case Y_DOWN:
        {
            if(!Y && !sharp && !X) 
            {
                state = UNLOCK; 
            }
            else if(Y && (sharp || X))
            {
                state = WAIT; 
            }
            else if(Y && !sharp && !X)
            {
                state = Y_DOWN; 
            }

            break;
        }
        case UNLOCK:
        {
            if(inHouse) 
            {
                state = WAIT;  
            }
            else
            {
                state = UNLOCK; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case WAIT:
        case SHARP_DOWN:
        case SHARP_RELEASE:
        case Y_DOWN: door = LOCKED; break;
        case UNLOCK: door = UNLOCKED; break;
        default: door = LOCKED;
    }

    PORTB = door;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    door = LOCKED;
    state = WAIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
