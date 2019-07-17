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

// buttons
#define up (~PINA & 0x01)
#define down (~PINA & 0x02)
#define start (~PINA & 0x04)

// tasks
#define NUM_TASKS 3

const unsigned long tasksPeriod = 50;

typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

Task gameTask;
Task scrollTask; // scrolls obstacles
Task playerTask; // handles player movement
Task *tasks[NUM_TASKS] = {
    &gameTask,
    &scrollTask,
    &playerTask
};

enum GamePaused
{
    FALSE,
    TRUE
} gamePaused;

unsigned char playerPos;

#define LCD_ROW_SIZE 16  
#define ROW1_END 15 
#define ROW2_END 33
#define ROW1_BEGIN 1
#define ROW2_BEGIN 17
#define COLUMN_END 15

enum ScrollStates
{
    NSTART,
    SCROLL_INIT,
    SCROLL,
    GAME_OVER,
    RESET
};

#define OBSTACLE_SIZE 35

const unsigned char topObstacle[OBSTACLE_SIZE + 1] = "    #    #     #     #     #      #";
const unsigned char bottomObstacle[OBSTACLE_SIZE + 1] = " #    #     #     #     #      #   ";

// keep track of the current symbols on bottom and top
unsigned char currBottomSym = ' ';
unsigned char currTopSym = ' ';

int scrollTick(int state)
{
    static unsigned char topString[LCD_ROW_SIZE + 1];
    static unsigned char bottomString[LCD_ROW_SIZE + 1];
    static unsigned char colPosition; // position of column
    unsigned char i;

    // transitions
    switch(state)
    {
        case NSTART: state = SCROLL_INIT; break;
        case SCROLL_INIT: 
        {
            if(gamePaused == TRUE)
            {
                state = SCROLL_INIT; 
            }
            else
            {
                state = SCROLL; 
            }
            
            break;
        }
        case SCROLL:
        {
            if((playerPos == ROW1_BEGIN && currTopSym == '#') ||
                (playerPos == ROW2_BEGIN && currBottomSym == '#'))
            {
                state = GAME_OVER; 
            }
            
            break;
        }
        case GAME_OVER: 
        {
            if(start)
            {
                state = GAME_OVER; 
            }
            else
            {
                state = RESET; 
            }

            break; 
        }
        case RESET:
        {
            if(start) 
            {
                state = SCROLL_INIT; 
            }
            else
            {
                state = RESET; 
            }

            break;
        }
    }

    switch(state)
    {
        case NSTART: break;
        case SCROLL_INIT:
        {
            LCD_ClearScreen();
            playerPos = ROW1_BEGIN;
            colPosition = 0;
            LCD_Cursor(playerPos);

            // set temp string empty
            strcpy(topString, "                ");
            strcpy(bottomString, "                ");

            break;
        }
        case SCROLL:
        {
            if(gamePaused == FALSE)
            {
                // write current part of message that fits on LCD screen
                for(i = 0; i < LCD_ROW_SIZE; ++i)
                {
                    LCD_Cursor(i + ROW1_BEGIN);
                    LCD_WriteData(topString[i]);
                    LCD_Cursor(i + ROW2_BEGIN);
                    LCD_WriteData(bottomString[i]);
                }

                LCD_Cursor(playerPos);

                if(colPosition < OBSTACLE_SIZE)
                {
                    // move all characters on LCD screen to the left
                    memmove(topString, topString + 1, 15);
                    topString[COLUMN_END] = topObstacle[colPosition];
                    memmove(bottomString, bottomString + 1, 15);
                    bottomString[COLUMN_END] = bottomObstacle[colPosition];

                    colPosition++;
                }
                else // reset position to 0
                {
                    colPosition = 0; 
                }
            } 

            currBottomSym = bottomString[0];
            currTopSym = topString[0];

            break;
        }
        case GAME_OVER:
        {
            gamePaused = TRUE;

            LCD_ClearScreen();
            LCD_DisplayString(1, "Game Over");
            
            break;
        }
        case RESET: break;
    }

    return state;
}

enum PlayerStates
{
    WAIT_SIGNAL,
    UP_PRESS,
    UP_HELD,
    DOWN_PRESS,
    DOWN_HELD
};

int playerTick(int state)
{
    // transitions
    switch(state)
    {
        case WAIT_SIGNAL:
        {
            if(up && !down)
            {
                state = UP_PRESS; 
            }
            else if(!up && down)
            {
                state = DOWN_PRESS;
            }
            else
            {
                state = WAIT_SIGNAL; 
            }

            break;
        }
        case UP_PRESS:
        {
            if(up && !down)
            {
                state = UP_HELD; 
            }
            else
            {
                state = WAIT_SIGNAL; 
            }

            break;
        }
        case UP_HELD:
        {
            if(up && !down)
            {
                state = UP_HELD; 
            }
            else
            {
                state = WAIT_SIGNAL; 
            }

            break;
        }
        case DOWN_PRESS:
        {
            if(!up && down)
            {
                state = DOWN_HELD; 
            }
            else
            {
                state = WAIT_SIGNAL; 
            }

            break;
        }
        case DOWN_HELD:
        {
            if(!up && down)
            {
                state = DOWN_HELD; 
            }
            else
            {
                state = WAIT_SIGNAL; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case WAIT_SIGNAL: break;
        case UP_PRESS: playerPos = (ROW1_BEGIN); break;
        case UP_HELD: break;
        case DOWN_PRESS: playerPos = (ROW2_BEGIN); break;
        case DOWN_HELD: break;
    }

    return state;
}

enum gameStates
{
    INIT,
    START,
    IN_START,
    PAUSE,
    IN_PAUSE
};

int gameTick(int state)
{
    // transitions
    switch(state) 
    {
        case INIT: state = start ? START : INIT; break;
        case START:
        {
            if(start) 
            {
                state = START;
            }
            else
            {
                state = IN_START; 
            }
            
            break;
        }
        case IN_START:
        {
            if(start) 
            {
                state = PAUSE; 
            }
            else
            {
                state = IN_START; 
            }

            break;
        }
        case PAUSE:
        {
            if(start) 
            {
                state = PAUSE; 
            }
            else
            {
                state = IN_PAUSE; 
            }

            break;
        }
        case IN_PAUSE:
        {
            if(start) 
            {
                state = START; 
            }
            else
            {
                state = IN_PAUSE; 
            }
        
            break; 
        }
    }

    // actions
    switch(state)
    {
        case INIT:
        {
            gamePaused = TRUE;
            break;
        }
        case START:
        {
            gamePaused = FALSE; 

            break;
        }
        case PAUSE:
        {
            gamePaused = TRUE;

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
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    TimerSet(tasksPeriod);
    TimerOn();

    scrollTask.state = START;
    scrollTask.period = 250;
    scrollTask.elapsedTime = scrollTask.period;
    scrollTask.TickFct = &scrollTick;

    playerTask.state = WAIT_SIGNAL;
    playerTask.period = 50;
    playerTask.elapsedTime = playerTask.period;
    playerTask.TickFct = &playerTick;

    gameTask.state = INIT;
    gameTask.period = 50;
    gameTask.elapsedTime = gameTask.period;
    gameTask.TickFct = &gameTick;

    while (1)
    {
    }

    return 0;
}

