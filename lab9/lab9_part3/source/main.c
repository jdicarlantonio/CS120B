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

#include "../header/time.h"

#define B1 (~PINA & 0x01)

// timer
const unsigned long halfsec = 10;
const unsigned long sec1 = 20;
const unsigned long sec5 = 100;
const unsigned long period = 50;

// note freq
#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25

float melody[8] = {
    D4, F4, E4, C4, D4, F4, E4, G4
};
unsigned char melodyIndex = 0;
unsigned char count = 0; // for timing

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

enum NoiseStates
{
    NOISE_OFF,
    NOISE_ON,
    WAIT_FOR_RELEASE
} noiseState;

void noiseTick()
{
    // transitions
    switch(noiseState)
    {
        case NOISE_OFF:
        {
            if(!B1) 
            {
                noiseState = NOISE_OFF;
            }
            else if(B1)
            {
                noiseState = NOISE_ON; 
            }

            break;
        }
        case NOISE_ON:
        {
            if(count < sec5) 
            {
                noiseState = NOISE_ON; 
            }
            else if(B1)
            {
                noiseState = WAIT_FOR_RELEASE; 
            }
            else if(!B1)
            {
                noiseState = NOISE_OFF; 
            }

            break;
        }
        case WAIT_FOR_RELEASE: 
        {
            noiseState = B1 ? WAIT_FOR_RELEASE : NOISE_OFF;

            break;
        }
    }

    // actions
    switch(noiseState)
    {
        case NOISE_OFF:
        {
            set_PWM(0.0f);
            melodyIndex = 0;
            count = 0;

            break;
        }
        case NOISE_ON:
        {
            ++count; 

            if(count % 5 == 0 && melodyIndex == 3)
            {
                set_PWM(melody[melodyIndex]);
            }

            if(count % halfsec == 0)
            {
                set_PWM(melody[melodyIndex++]);

                if(melodyIndex >= 8)
                {
                     melodyIndex = 0;
                }
            }

            break;
        }
        case WAIT_FOR_RELEASE: 
        {
            set_PWM(0.0f);

            break;
        }
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(period);
    TimerOn();

    noiseState = NOISE_OFF;
    count = 0;
    melodyIndex = 0;
    set_PWM(0.0f);
    PWM_on();
    while (1) 
    {
        noiseTick();

        while(!TimerFlag) {}
        TimerFlag = 0;
    }
    return 1;
}
