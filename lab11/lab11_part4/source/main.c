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
#include "../header/io.h"

// tasks
#define NUM_TASKS 1

const unsigned long tasksPeriod = 50;

typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

Task keypadTask;
Task *tasks[NUM_TASKS] = {
    &keypadTask
};

enum KeypadStates
{
    START,
    INIT,
    READ_KEY,
    KEY_HELD
};

int keypadTick(int state)
{
    static unsigned char keypadKey;
    static unsigned char cursor;

    // transitions
    switch(state)
    {
        case START: state = INIT; break;
        case INIT: state = READ_KEY; break;
        case READ_KEY: 
        {
            if(GetKeypadKey() != '\0')
            {
                keypadKey = GetKeypadKey();
                LCD_Cursor(cursor);
                LCD_WriteData(keypadKey);

                cursor++;
                if(cursor > 16)
                {
                    cursor = 1; 
                }

                state = KEY_HELD; 
            }
            else
            {
                state = READ_KEY; 
            }
            
            break;
        }
        case KEY_HELD:
        {
            if(GetKeypadKey() == '\0') 
            {
                state = READ_KEY; 
            }
            else
            {
                state = KEY_HELD; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case START: break;
        case INIT: 
        {
            keypadKey = '\0'; 
            cursor = 1;
            LCD_DisplayString(1, "Congratulations!");

            break;
        }
        case READ_KEY: 
        {
            break;
        }
        case KEY_HELD:
        {
            break; 
        }
    }

    return state;
}

void TimerISR()
{
    unsigned char i;
    for(i = 0; i < NUM_TASKS; ++i)
    {
        if(tasks[i]->elapsedTime >= tasks[i]->period) 
        {
            tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
            tasks[i]->elapsedTime = 0;
        }
        tasks[i]->elapsedTime += tasksPeriod;
    }
}

int main(void) 
{ 
    DDRA = 0xF0; PORTA = 0x0F;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    TimerSet(tasksPeriod);
    TimerOn();

    keypadTask.state = START;
    keypadTask.period = 50;
    keypadTask.elapsedTime = keypadTask.period;
    keypadTask.TickFct = &keypadTick;

    while (1)
    {
    }

    return 0;
}

