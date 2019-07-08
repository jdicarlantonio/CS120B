/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include "../header/time.h"
#include "../header/io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define button (~PINA & 0x01)
#define out PORTB

#define period 200

char score = 0;

enum Scored
{
    FALSE, // 0
    TRUE,  // 1
    NO_SCORE
} scored;

enum States
{
    START,
    LED1,
    LED2, 
    LED3,
    BUTTON_PRESS,
    BUTTON_RELEASE,
    RESTART
} state;

void gameSM()
{
    // transitions
    switch(state)
    {
        case START: state = LED1; break;
        case LED1:
        {
            if(button)
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED2; 
            }

            break;
        }
        case LED2: 
        {
            if(button) 
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED3; 
            }

            break;
        }
        case LED3:
        {
            if(button) 
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = LED1; 
            }

            break;
        }
        case BUTTON_PRESS:
        {
            if(button)
            {
                state = BUTTON_PRESS; 
            }
            else
            {
                state = BUTTON_RELEASE; 
            }

            break;
        }
        case BUTTON_RELEASE:
        {
            if(button)
            {
                if(score >= 9)
                {
                    score = 0; 
                            
                    LCD_DisplayString(1, "Score:");
                    LCD_Cursor(7);
                    LCD_WriteData(score + '0');
                }

                state = RESTART; 
            }
            else
            {
                state = BUTTON_RELEASE; 
            }

            break;
        }
        case RESTART:
        {
            if(button) 
            {
                state = RESTART; 
            }
            else
            {
                state = LED1; 
            }
        }
    }

    // actions
    switch(state)
    {
        case START:
        case LED1: 
        {
            out = 0x01; 
            scored = FALSE;

            break;
        }
        case LED2: 
        {
            out = 0x02; 
            scored = TRUE;    
            
            break;
        }
        case LED3: 
        {
            out = 0x04; 
            scored = FALSE;    
            
            break;
        }
        case BUTTON_PRESS:
        {
            if(scored == TRUE)
            {
                score++; 

                scored = NO_SCORE;
            }
            if(scored == FALSE)
            {
                score--;
                if(score <= 0) score = 0;

                scored = NO_SCORE;
            }

            break;
        }
        case BUTTON_RELEASE: 
        {
            if(score >= 9) 
            {
                LCD_ClearScreen();
                LCD_DisplayString(1, "You Won!!!");
            }
        
            break;
        }
        case RESTART: break;
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0xFF; PORTC = 0x00;

    TimerSet(period);
    TimerOn();

    LCD_init();
    LCD_DisplayString(1, "Score:");

    state = START;
    while (1) 
    {
        gameSM();

        if(score < 9)
        {
            LCD_Cursor(7);
            LCD_WriteData(score + '0');
        }

        while(!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
