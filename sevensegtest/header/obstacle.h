#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "customChar.h"
#include "playerMovement.h"
#include "playerShots.h"

#define LCD_ROW_SIZE 15
#define OBSTACLE_BEGIN 2
#define OBSTACLE_SIZE 35

unsigned char enemiesKilled = 0;
unsigned char friendliesNeglected = 0;

unsigned char obstacle[OBSTACLE_SIZE] = {
    0x20, 0x20, 0x01, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x01, 0x20, 
    0x20, 0x20, 0x20, 0x04, 0x20,
    0x20, 0x20, 0x01, 0x01, 0x20,
    0x20, 0x20, 0x04, 0x04, 0x20,
    0x20, 0x20, 0x20, 0x01, 0x04,
    0x20, 0x20, 0x01, 0x20, 0x04
};

enum ScrollStates
{
    SC_START, 
    SC_INIT,
    SCROLL
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

int scrollTick(int state)
{
    static unsigned char tmpObstacle[LCD_ROW_SIZE + 1];
    static unsigned char position = 0;
    static unsigned char crosshairPos;
    unsigned char i;

    // transitions
    switch(state)
    {
        case SC_START: state = SC_INIT; break;
        case SC_INIT: state = SCROLL; break;
        case SCROLL: state = SCROLL; break;
    }

    // actions
    switch(state)
    {
        case SC_START: break;
        case SC_INIT:
        {
            position = 0;
            crosshairPos = 15;
            strcpy(tmpObstacle, "               ");

            break;
        }
        case SCROLL:
        {
            crosshairPos = getNextSprite(tmpObstacle, LCD_ROW_SIZE);
            if(potentialHit && (tmpObstacle[crosshairPos] == MONSTER))
            {
                potentialHit = 0;
                enemiesKilled++;
                tmpObstacle[crosshairPos] = ' ';
            }
            else
            {
                potentialHit = 0; 
            }

            if(tmpObstacle[0] == FRIENDLY && !ducking)
            {
                friendliesNeglected++;
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
            

            break; 
        }
    }

    return state;
}

#endif
