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

#define SWITCH (PINA & 0x04)

#define out PORTB

#define numTasks 4
unsigned long tasksPeriod = 1;

// shared variables
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char noise;

enum SpeakerNoise
{
    NSTART,
    NOISE_OFF,
    NOISE_ON
} speakerState;

int SpeakerNoiseTick(int state)
{
    // transition
    switch(state) 
    {
        case NSTART: state = NOISE_OFF; break;
        case NOISE_OFF: state = SWITCH ? NOISE_ON : NOISE_OFF; break;
        case NOISE_ON: state = SWITCH ? NOISE_OFF : NOISE_ON; break;
    }

    // action
    switch(state)
    {
        case NSTART:
        case NOISE_OFF: noise = 0x00; break;
        case NOISE_ON: noise = 0x10; break;
    }

    return state;
}

enum ThreeLED
{
    START,
    LED0,
    LED1,
    LED2
} threeLEDState;

int ThreeLEDTick(int state)
{
    // transitions
    switch(state)
    {
        case START: state = LED0; break;
        case LED0: state = LED1; break;
        case LED1: state = LED2; break;
        case LED2: state = LED0; break;
    }

    // actions
    switch(state)
    {
        case START:
        case LED0: threeLEDs = 0x01; break;
        case LED1: threeLEDs = 0x02; break;
        case LED2: threeLEDs = 0x04; break;
    }

    return state;
}

enum BlinkingLED
{
    BSTART,
    LED_OFF,
    LED_ON
} blinkLEDState;

int BlinkingTick(int state)
{
    // transitions
    switch(state)
    {
        case BSTART: state = LED_OFF; break;
        case LED_OFF: state = LED_ON; break;
        case LED_ON: state = LED_OFF; break;
    }

    // actions
    switch(state)
    {
        case BSTART:
        case LED_OFF: blinkingLED = 0x00; break;
        case LED_ON: blinkingLED = 0x08; break;
    }

    return state;
}

enum CombineLED
{
    COMBINE_LED
} combineLEDState;

int CombineTick(int state)
{
    // transistions
    switch(state)
    {
        case COMBINE_LED: state = COMBINE_LED; break;
    }

    // actions
    switch(state)
    {
        case COMBINE_LED: out = noise | threeLEDs | blinkingLED; break;
    }

    return state;
}

typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

Task tasks[numTasks];

// some defines for easier indexing
#define THREE_LEDS 0
#define BLINK_LED 1
#define COMBINE 2
#define NOISE 3

void TimerISR()
{
    unsigned char i;
    for(i = 0; i < numTasks; ++i)
    {
        if(tasks[i].elapsedTime >= tasks[i].period) 
        {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriod;
    }
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;

    tasks[NOISE].state = NSTART;
    tasks[NOISE].period = 2;
    tasks[NOISE].elapsedTime = tasks[NOISE].period;
    tasks[NOISE].TickFct = &SpeakerNoiseTick;

    tasks[THREE_LEDS].state = START;
    tasks[THREE_LEDS].period = 300;
    tasks[THREE_LEDS].elapsedTime = tasks[THREE_LEDS].period;
    tasks[THREE_LEDS].TickFct = &ThreeLEDTick;
    
    tasks[BLINK_LED].state = BSTART;
    tasks[BLINK_LED].period = 1000;
    tasks[BLINK_LED].elapsedTime = tasks[BLINK_LED].period;
    tasks[BLINK_LED].TickFct = &BlinkingTick;
    
    tasks[COMBINE].state = COMBINE_LED;
    tasks[COMBINE].period = 1;
    tasks[COMBINE].elapsedTime = tasks[COMBINE].period;
    tasks[COMBINE].TickFct = &CombineTick;
    
    TimerSet(tasksPeriod);
    TimerOn();

    while (1) 
    {
    }

    return 0;
}
