/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab 11  Exercise example
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
#include "../header/keypad.h"
#include "../header/time.h"

// timer globals
const unsigned long tasksPeriod = 50;

// task structure
#define NUM_TASKS 1
typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

unsigned char keypadKey;

enum KeypadStates
{
    START,
    INIT,
    READ_KEY
} keypadState;

int keypadTick(int state)
{
    // transitions
    switch(state)
    {
        case START: state = INIT; break;
        case INIT: state = READ_KEY; break;
        case READ_KEY: state = READ_KEY; break;
    }

    // actions
    switch(state)
    {
        case START: break;
        case INIT: keypadKey = '\0'; break;
        case READ_KEY: keypadKey = GetKeypadKey(); break;
    }

    return state;
}

// tasks
Task tasks[NUM_TASKS];
#define KEYPAD_TASK 0

void TimerISR()
{
    unsigned char i;
    for(i = 0; i < NUM_TASKS; ++i)
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
    DDRA = 0xF0; PORTA = 0x0F;

    TimerSet(tasksPeriod);
    TimerOn();
    
    tasks[KEYPAD_TASK].state = START;
    tasks[KEYPAD_TASK].period = 50;
    tasks[KEYPAD_TASK].elapsedTime = tasks[KEYPAD_TASK].period;
    tasks[KEYPAD_TASK].TickFct = &keypadTick;

    while (1)
    {
        switch(keypadKey)
        {
            case '\0': PORTB = 0x1F; break;
            case '1': PORTB = 0x01; break;
            case '2': PORTB = 0x02; break;
            case '3': PORTB = 0x03; break;
            case '4': PORTB = 0x04; break;
            case '5': PORTB = 0x05; break;
            case '6': PORTB = 0x06; break;
            case '7': PORTB = 0x07; break;
            case '8': PORTB = 0x08; break;
            case '9': PORTB = 0x09; break;
            case 'A': PORTB = 0x0A; break;
            case 'B': PORTB = 0x0B; break;
            case 'C': PORTB = 0x0C; break;
            case 'D': PORTB = 0x0D; break;
            case '*': PORTB = 0x0E; break;
            case '0': PORTB = 0x00; break;
            case '#': PORTB = 0x0F; break;
            default: PORTB = 0x1B; break;
        }
    }

    return 0;
}

