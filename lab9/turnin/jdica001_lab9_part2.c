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

// user includes
#include "../header/time.h"

#define period 50

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

// current frequency
float freq[8] = {
    C4, D4, E4, F4, G4, A4, B4, C5
};
int scaleIndex = 0;

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

enum SoundStates
{
    NO_SOUND,
    BUTTON_PRESSED_ON,
    BUTTON_PRESSED_OFF,
    SOUND_ON
} soundState;

enum FreqStates
{
    WAIT_BUTTON,
    INC,
    DEC
} freqState;

void incFreq()
{
    scaleIndex++;
    if(scaleIndex > 7)
    {
        scaleIndex = 7; 
    }
}

void decFreq()
{
    scaleIndex--;
    if(scaleIndex < 0)
    {
        scaleIndex = 0; 
    }
}

void noiseTick()
{
    // transitions
    switch(soundState)
    {
        case NO_SOUND:
        {
            if(!B1) 
            {
                soundState = NO_SOUND; 
            }
            else if(B1)
            {
                soundState = BUTTON_PRESSED_ON; 
            }

            break;
        }
        case BUTTON_PRESSED_ON:
        {
            if(B1) 
            {
                soundState = BUTTON_PRESSED_ON; 
            }
            else if(!B1)
            {
                soundState = SOUND_ON; 
            }

            break;
        }
        case BUTTON_PRESSED_OFF:
        {
            if(B1) 
            {
                soundState = BUTTON_PRESSED_OFF; 
            }
            else if(!B1)
            {
                soundState = NO_SOUND; 
            }

            break;
        }
        case SOUND_ON:
        {
            if(!B1) 
            {
                soundState = SOUND_ON; 
            }
            else if(B1)
            {
                soundState = BUTTON_PRESSED_OFF; 
            }

            break;
        }
    }

    // actions
    switch(soundState)
    {
        case NO_SOUND: set_PWM(0.0f); break;
        case BUTTON_PRESSED_ON: break;
        case BUTTON_PRESSED_OFF: break;
        case SOUND_ON: set_PWM(freq[scaleIndex]); break;
    }
}

void freqTick()
{
    // transitions
    switch(freqState)
    {
        case WAIT_BUTTON:
        {
            if(B2 && !B3)
            {
                incFreq();
                freqState = INC; 
            }
            else if(B3 && !B2)
            {
                decFreq();
                freqState = DEC; 
            }
            else
            {
                freqState = WAIT_BUTTON; 
            }

            break;
        }
        case INC:
        {
            if(B2 && !B3) 
            {
                freqState = INC; 
            }
            else 
            {
                freqState = WAIT_BUTTON; 
            }

            break;
        }
        case DEC:
        {
            if(B3 && !B2) 
            {
                freqState = DEC; 
            }
            else
            {
                freqState = WAIT_BUTTON; 
            }

            break;
        }
    }

    // actions
    switch(freqState)
    {
        case WAIT_BUTTON: break;
        case INC: break;
        case DEC: break;
    }
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    
    TimerSet(period);
    TimerOn();

    set_PWM(0);
    PWM_on();
    while (1) 
    {
        freqTick();
        noiseTick();

        while(!TimerFlag) {}
        TimerFlag = 0;
    }

    return 1;
}
