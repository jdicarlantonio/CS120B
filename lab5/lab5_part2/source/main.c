/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define button1 (~PINA & 0x01)
#define button2 (~PINA & 0x02)

enum ValueStates
{
    WAIT,
    INCREMENT,
    DECREMENT,
    RESET
} state;

void tick(char *value)
{
    // transitions
    switch(state)
    {
        case WAIT:
        {
            if(button1 && !button2)
            {
                (*value)++;
                /*
                if(*value >= 0x09)
                {
                    *value = 0x09;
                }
                */
                state = INCREMENT;
            }
            else if(!button1 && button2)
            {
                (*value)--;
                /*
                if(*value <= 0x00)
                {
                    *value = 0x00;
                }
                */
                state = DECREMENT;
            }
            else if(button1 && button2)
            {
                *value = 0x00;
                state = RESET;
            }
            else
            {
                state = WAIT;
            }
            
            break;
        }
        case INCREMENT:
        {
            if(button1 && !button2)
            {
                state = INCREMENT;
            }
            else if(!button1 && !button2)
            {
                state = WAIT;
            }
            else if(button1 && button2)
            {
                *value = 0x00;
                state = RESET;
            }

            break;
        }
        case DECREMENT:
        {
            if(button2 && !button1)
            {
                state = DECREMENT;
            }
            else if(!button2 && !button1)
            {
                state = WAIT;
            }
            else if(button2 && button1)
            {
                *value = 0x00;
                state = RESET;
            }
            
            break;
        }
        case RESET:
        {
            if(button1 || button2)
            {
                state = RESET;
            }
            else
            {
                state = WAIT;
            }
            
            break;
        }
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    char value = 0x00;
    state = WAIT;
    while (1) 
    {
        tick(&value);

        if(value >= 0x09)
        {
            value = 0x09;
        }
        if(value <= 0x00)
        {
            value = 0x00;
        }

        PORTC = value;
    }
    return 1;
}
