#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "customChar.h"
#include "io.h" 

#define R (~PINA & 0x10) // right
#define D (~PINA & 0x04) // duck, or down
#define ROW1_BEGIN 1
#define ROW2_BEGIN 17

unsigned char inMenu;

unsigned long playerMovementPeriod = 50;
unsigned long ms250 = 5;

enum MovementFlag
{
    FALSE,
    TRUE
} ducking;

enum PlayerMovementStates
{
    WAIT,
//    MOVE_RIGHT, // moves player over once
//    SCROLL_RIGHT, // continues to scroll player
    DUCK // for dodging, ducking, dipping, diving, and dodging
};

void spriteUpdate(unsigned char currSprite)
{
    LCD_Cursor(ROW1_BEGIN);
    LCD_WriteData(currSprite);
}

int playerMovementTick(int state)
{
    static unsigned long count = 0;
    static unsigned char sprite = 0; 

    // transistions
    switch(state)
    {
        case WAIT:
        {
            if(inMenu == 0)
            {
                /*
                if(R && !D) 
                {
                    state = MOVE_RIGHT; 
                } */
                if(D)
                {
                    state = DUCK; 

                    LCD_Cursor(ROW1_BEGIN);
                    LCD_WriteData(' ');
                }
                else
                {
                    state = WAIT; 
                }
            }
            break;
        }
/*        case MOVE_RIGHT:
        {
            if(R && !D) 
            {
                state = SCROLL_RIGHT; 
            }
            else
            {
                state = WAIT;
            }

            break;
        }
        case SCROLL_RIGHT:
        {
            if(R && !D) 
            {
                state = SCROLL_RIGHT; 
            }
            else
            {
                // character must not be moving to duck, so if the player
                // presses duck while moving, the character will stop moving
                // first and the player will have to press duck again
                state = WAIT; 
            }

            break;
        } */
        case DUCK:
        {
            if(D) 
            {
                state = DUCK; 
            }
            else
            {
                state = WAIT;
                
                // clear the bottom row character and repaint on top row
                LCD_Cursor(ROW2_BEGIN);
                LCD_WriteData(' ');
                LCD_Cursor(ROW1_BEGIN);
                LCD_WriteData(PL_STATIONARY);
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case WAIT: 
        {
            count = 0; 
//            walking = FALSE;
            ducking = FALSE;
            
            break;
        }
/*        case MOVE_RIGHT:
        {
            walking = TRUE;
            ducking = FALSE;

            spriteUpdate(sprite);
//            scrollUpdate();

            break;
        }
        case SCROLL_RIGHT:
        {
            walking = TRUE;
            ducking = FALSE;
            count++; 

            if(count == ms250)
            {
                count = 0;

                spriteUpdate(sprite);
//                scrollUpdate();
            }

            break;
        } */
        case DUCK:
        {
//            walking = FALSE;
            ducking = TRUE;
            
            LCD_Cursor(ROW2_BEGIN);
            LCD_WriteData(PL_KNEEL);

            break;
        }
    }

    return state;
}

#endif
