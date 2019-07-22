#ifndef GAME_H
#define GAME_H

#include <avr/eeprom.h>

#include "customChar.h"
#include "playerMovement.h"
#include "playerShots.h"

#define start (~PINA & 0x08)

#define LCD_ROW_SIZE 15
#define OBSTACLE_BEGIN 2
#define OBSTACLE_SIZE 35

unsigned char currentScore;
//unsigned char inMenu;

unsigned char obstacle[OBSTACLE_SIZE] = {
    0x20, 0x20, 0x01, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x01, 0x20, 
    0x20, 0x20, 0x20, 0x04, 0x20,
    0x20, 0x20, 0x01, 0x01, 0x20,
    0x20, 0x20, 0x04, 0x04, 0x20,
    0x20, 0x20, 0x20, 0x01, 0x04,
    0x20, 0x20, 0x01, 0x20, 0x04
};

enum GameStates
{
    SC_START, 
    SC_INIT,
    SCROLL,
    MENU,
    START_HELD
};

unsigned char getNextSprite(unsigned char* spriteSheet, unsigned long sz)
{
    unsigned char index = 0;

    unsigned char i;
    for(i = 0; i < sz; ++i)  
    {
        if(spriteSheet[i] != 0x20)
        {
            index = i; 
            break;
        }
    }

    return index;
}

int gameTick(int state)
{
    static unsigned char tmpObstacle[LCD_ROW_SIZE + 1];
    static unsigned char position = 0;
    static unsigned char crosshairPos;
    unsigned char i;

    // transitions
    switch(state)
    {
        case SC_START: state = SC_INIT; break;
        case SC_INIT: 
        {
            //LCD_ClearScreen();
            LCD_DisplayString(5, "Monster       Press Start");
            state = MENU; 
            break;
        }
        case SCROLL: 
        {
            // start button is same as reset
            if(start) 
            {
                state = SC_INIT; 
            }
            else
            {
                state = SCROLL; 
            }
            
            break;
        }
        case MENU:
        {
            if(start) 
            {
                inMenu = 0;
                state = START_HELD; 
            }
            else
            {
                inMenu = 1;
                state = MENU; 
            }

            break;
        }
        case START_HELD:
        {
            if(start) 
            {
                state = START_HELD; 
            }
            else
            {
                LCD_ClearScreen();

                // redraw player
                LCD_Cursor(1);
                LCD_WriteData(PL_STATIONARY);

                state = SCROLL; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case SC_START: break;
        case SC_INIT:
        {
            position = 0;
            crosshairPos = 15;
            currentScore = 0;
            strcpy(tmpObstacle, "               ");
            inMenu = 1;

            break;
        }
        case SCROLL:
        {
            inMenu = 0;
            crosshairPos = getNextSprite(tmpObstacle, LCD_ROW_SIZE);
            if(potentialHit && (tmpObstacle[crosshairPos] == MONSTER))
            {
                potentialHit = 0;
                currentScore++;
                tmpObstacle[crosshairPos] = ' ';
            }
            else
            {
                potentialHit = 0; 
            }

            if(tmpObstacle[0] == FRIENDLY && !ducking)
            {
                if(currentScore > 0)
                {
                    currentScore--;
                }
            }

            activeBulletSpace = getNextSprite(tmpObstacle, LCD_ROW_SIZE);
            for(i = 0; i < LCD_ROW_SIZE; ++i)
            {
                LCD_Cursor(i + OBSTACLE_BEGIN);
                LCD_WriteData(tmpObstacle[i]);
            }
           
            if(position < OBSTACLE_SIZE)
            {
                memmove(tmpObstacle, tmpObstacle + 1, LCD_ROW_SIZE - 1);
                tmpObstacle[LCD_ROW_SIZE - 1] = obstacle[position];

                ++position;
            }
            else
            {
                position = 0; 
            }
            
            LCD_DisplayDigit(31, currentScore);
            
            break; 
        }
        case MENU:
        {
            inMenu = 1; 

            activeBulletSpace = 0;
//            LCD_DisplayString(19, "Press Start");

            break;
        }
        case START_HELD: break;
    }

    return state;
}

#endif
