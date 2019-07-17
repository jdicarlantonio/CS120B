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

#define out PORTB

#define numTasks 3
unsigned long tasksPeriod = 1000;

// shared variables
unsigned char threeLEDs;
unsigned char blinkingLED;

enum ThreeLED
{
    LED0,
    LED1,
    LED2
} threeLEDState;

void ThreeLEDTick()
{
    // transitions
    switch(threeLEDState)
    {
        case LED0: threeLEDState = LED0; break;
        case LED1: threeLEDState = LED1; break;
        case LED2: threeLEDState = LED2; break;
    }

    // actions
    switch(threeLEDState)
    {
        case LED0: threeLEDs = 0x01; break;
        case LED1: threeLEDs = 0x02; break;
        case LED2: threeLEDs = 0x04; break;
    }
}

enum BlinkingLED
{
    LED_OFF,
    LED_ON
} blinkLEDState;

void BlinkingTick()
{
    // transitions
    switch(blinkLEDState)
    {
        case LED_OFF: blinkLEDState = LED_ON; break;
        case LED_ON: blinkLEDState = LED_OFF; break;
    }

    // actions
    switch(blinkLEDState)
    {
        case LED_OFF: blinkingLED = 0x00; break;
        case LED_ON: blinkingLED = 0x08; break;
    }
}

enum CombineLED
{
    COMBINE_LED
} combineLEDState;

void CombineTick()
{
    // transistions
    switch(combineLEDState)
    {
        case COMBINE_LED: combineLEDState = COMBINE_LED; break;
    }

    // actions
    switch(combineLEDState)
    {
        case COMBINE_LED: out = threeLEDs | blinkingLED; break;
    }
}
/*
typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

Task tasks[numTasks];
*/
// some defines for easier indexing
#define THREE_LEDS 0
#define BLINK_LED 1
#define COMBINE 2

void TimerISR()
{
/*
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
*/
    TimerFlag = 1;
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;

/*
    tasks[THREE_LEDS].state = LED0;
    tasks[THREE_LEDS].period = 1000;
    tasks[THREE_LEDS].elapsedTime = tasks[THREE_LEDS].period;
    tasks[THREE_LEDS].TickFct = &ThreeLEDTick;
    
    tasks[BLINK_LED].state = LED_OFF;
    tasks[BLINK_LED].period = 1000;
    tasks[BLINK_LED].elapsedTime = tasks[BLINK_LED].period;
    tasks[BLINK_LED].TickFct = &BlinkingTick;
    
    tasks[COMBINE].state = COMBINE_LED;
    tasks[COMBINE].period = 1000;
    tasks[COMBINE].elapsedTime = tasks[COMBINE].period;
    tasks[COMBINE].TickFct = &BlinkingTick;
*/   
    TimerSet(tasksPeriod);
    TimerOn();

    threeLEDState = LED0;
    blinkLEDState = LED_OFF;
    combineLEDState = COMBINE_LED;
    while (1) 
    {
        ThreeLEDTick();
        BlinkingTick();
        CombineTick();

        while(!TimerFlag) {}
        TimerFlag = 0;
    }

    return 0;
}
