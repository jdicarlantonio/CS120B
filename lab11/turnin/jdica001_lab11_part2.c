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

// C includes
#include "string.h"

// user includes
#include "../header/keypad.h"
#include "../header/time.h"
#include "../header/io.h"

// timer globals
#define NUM_TASKS 1

const unsigned long tasksPeriod = 500;

typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

Task scrollTask;
Task *tasks[NUM_TASKS] = {
    &scrollTask
};


// scroll SM
#define LCD_ROW_SIZE 16  
#define LCD_LAST_INDEX 15 
#define MESG_SIZE 37

enum ScrollStates
{
    START,
    INIT,
    SCROLL
};

const unsigned char message[MESG_SIZE + 1] = "CS120B is Legend... wait for it DARY!";

int scrollTick(int state)
{
    static unsigned char string[LCD_ROW_SIZE + 1];
    static unsigned char currPosition;
    unsigned char i;

    // transitions
    switch(state)
    {
        case START: state = INIT; break;
        case INIT: state = SCROLL; break;
        case SCROLL: state = SCROLL; break;
    }

    switch(state)
    {
        case START: break;
        case INIT:
        {
            currPosition = 0;

            // set temp string empty
            strcpy(string, "                ");

            break;
        }
        case SCROLL:
        {
            LCD_Cursor(1);
           
            // write current part of message that fits on LCD screen
            for(i = 0; i < LCD_ROW_SIZE; ++i)
            {
                LCD_WriteData(string[i]);
            }

            if(currPosition < MESG_SIZE)
            {
                // move all characters on LCD screen to the left
                memmove(string, string + 1, LCD_LAST_INDEX);
                string[LCD_LAST_INDEX] = message[currPosition];

                currPosition++;
            }
            else if(currPosition < (MESG_SIZE + LCD_ROW_SIZE)) // if the message is done
            {
                // just add blank spaces 
                memmove(string, string + 1, LCD_LAST_INDEX);
                string[LCD_LAST_INDEX] = ' ';
                currPosition++;
            }
            else // reset position to 0
            {
                currPosition = 0; 
            }

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

    scrollTask.state = START;
    scrollTask.period = 500;
    scrollTask.elapsedTime = scrollTask.period;
    scrollTask.TickFct = &scrollTick;

    while (1)
    {
    }

    return 0;
}

