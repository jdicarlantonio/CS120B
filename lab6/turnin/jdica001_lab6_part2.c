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

#define button (~PINA & 0x01)
#define out PORTB

#define period 200

enum States
{
    START,
    LED1,
    LED2, 
    LED3,
    BUTTON_PRESS,
    BUTTON_RELEASE,
    RESTART
} state;

void gameSM()
{
    // transitions
    switch(state)
    {
        case START: state = LED1; break;
        case LED1:
        {
            if(button)
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED2; 
            }

            break;
        }
        case LED2: 
        {
            if(button) 
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED3; 
            }

            break;
        }
        case LED3:
        {
            if(button) 
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED1; 
            }

            break;
        }
        case BUTTON_PRESS:
        {
            if(button)
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = BUTTON_RELEASE; 
            }

            break;
        }
        case BUTTON_RELEASE:
        {
            if(button)
            {
                state = RESTART; 
            }
            else
            {
                state = BUTTON_RELEASE; 
            }

            break;
        }
        case RESTART:
        {
            if(button) 
            {
                state = RESTART; 
            }
            else
            {
                state = LED1; 
            }
        }
    }

    // actions
    switch(state)
    {
        case START:
        case LED1: out = 0x01; break;
        case LED2: out = 0x02; break;
        case LED3: out = 0x04; break;
        case BUTTON_PRESS: break;
        case BUTTON_RELEASE: break;
        case RESTART: break;
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(period);
    TimerOn();

    state = START;
    while (1) 
    {
        gameSM();
        
        while(!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
