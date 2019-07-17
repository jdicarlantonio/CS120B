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

// input
#define B1 (~PINA & 0x01)
#define B2 (~PINA & 0x02)
#define B3 (~PINA & 0x04)

// note freq
#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25

enum States
{
    NO_SOUND,
    PLAY_C4,
    PLAY_D4,
    PLAY_E4
} state;

void set_PWM(double frequency)
{
    static double current_frequency;

    if(frequency != current_frequency)
    {
        if(!frequency) 
        { 
            TCCR3B &= 0x08; 
        }
        else
        {
            TCCR3B |= 0x03; 
        }

        if(frequency < 0.954)
        {
            OCR3A = 0xFFFF; 
        }
        else if(frequency > 31250)
        {
            OCR3A = 0x0000; 
        }
        else
        {
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
        }

        TCNT0 = 0;
        current_frequency = frequency;

    }
}

void PWM_on()
{
    TCCR3A = (1 << COM0A0);
    TCCR3B = (1 << WGM02) | (1 << CS01) | (1 << CS00);

    set_PWM(0);
}

void PWM_off()
{
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

void noiseTick()
{
    // transition
    switch(state)
    {
        case NO_SOUND:
        {
            if(B1 && !B2 && !B3)
            {
                state = PLAY_C4; 
            }
            else if(B2 && !B1 && !B3)
            {
                state = PLAY_D4; 
            }
            else if(B3 && !B1 && !B2)
            {
                state = PLAY_E4; 
            }
            else
            {
                state = NO_SOUND;  
            }

            break;
        }
        case PLAY_C4:
        {
            if(B1 && !B2 && !B3) 
            {
                state = PLAY_C4; 
            }
            else 
            {
                state = NO_SOUND; 
            }

            break;
        }
        case PLAY_D4:
        {
            if(B2 && !B1 && !B3) 
            {
                state = PLAY_D4; 
            }
            else 
            {
                state = NO_SOUND; 
            }

            break;
        }
        case PLAY_E4:
        {
            if(B3 && !B1 && !B2) 
            {
                state = PLAY_E4; 
            }
            else 
            {
                state = NO_SOUND; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case NO_SOUND: 
        {
            set_PWM(0); 

            break;
        }
        case PLAY_C4:
        {
            set_PWM(C4);

            break;
        }
        case PLAY_D4:
        {
            set_PWM(D4);

            break;
        }
        case PLAY_E4:
        {
            set_PWM(E4);

            break;
        }
    }
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;

    state = NO_SOUND;

    set_PWM(0);
    PWM_on();
    while (1) 
    {
        noiseTick();
    }

    return 1;
}
