/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: Brandon Tran
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
#include "../header/io.h"
#include "../header/time.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define button1 (~PINA & 0x01)
#define button2 (~PINA & 0x02)

const unsigned long period = 50;
const unsigned long sec1 = 20;

// state machine variables
char value = 0;
unsigned long count = 0;

enum States
{
    WAIT_BUTTON,
    INC,
    INC_HELD,
    DEC,
    DEC_HELD,
    RESET
} state;

void tick()
{
    // transitions
    switch(state)
    {
        case WAIT_BUTTON:
        {
            if(!button1 && !button2) 
            {
                state = WAIT_BUTTON; 
            }
            else if(button1 && !button2)
            {
                state = INC; 
            }
            else if(!button1 && button2)
            {
                state = DEC; 
            }
            else
            {
                state = RESET; 
            }

            break;
        }
        case INC:
        {
            if(button1 && !button2) 
            {
                state = INC_HELD; 
            }
            else if(!button1 && !button2)
            {
                state = WAIT_BUTTON; 
            }
            else if(button1 && button2)
            {
                state = RESET; 
            }
            else
            {
                state = DEC; 
            }

            break;
        }
        case INC_HELD:
        {
            if(button1 && !button2) 
            {
                state = INC_HELD; 
            }
            else if(!button1 && !button2)
            {
                state = WAIT_BUTTON; 
            }
            else if(button1 && button2)
            {
                state = RESET; 
            }
            else
            {
                state = DEC; 
            }

            break;
        }
        case DEC:
        {
            if(button2 && !button1) 
            {
                state = DEC_HELD; 
            }
            else if(!button1 && !button2)
            {
                state = WAIT_BUTTON; 
            }
            else if(button1 && button2)
            {
                state = RESET; 
            }
            else
            {
                state = INC; 
            }
            
            break;
        }
        case DEC_HELD:
        {
            if(button2 && !button1) 
            {
                state = DEC_HELD; 
            }
            else if(!button1 && !button2)
            {
                state = WAIT_BUTTON; 
            }
            else if(button1 && button2)
            {
                state = RESET; 
            }
            else
            {
                state = INC; 
            }
            
            break;
        }
        case RESET:
        {
            if(button1 || button2) 
            {
                state = RESET; 
            }
            else
            {
                state = WAIT_BUTTON; 
            }

            break;
        }
    }

    // actions
    switch(state)
    {
        case WAIT_BUTTON: count = 0; break;
        case INC: 
        {
            value++;
            if(value >= 9)
            {
                value = 9; 
            }

            count++;
            LCD_Cursor(1);
            LCD_WriteData(value + '0');

            break;
        }
        case INC_HELD:
        {
            count++;
            if(count >= sec1)
            {
                count = 0;
                value++;
                if(value >= 9)
                {
                    value = 9; 
                }

                LCD_Cursor(1);
                LCD_WriteData(value + '0');
            }

            break;
        }
        case DEC: 
        {
            value--;
            if(value <= 0)
            {
                value = 0; 
            }

            count++;
            LCD_Cursor(1);
            LCD_WriteData(value + '0');

            break;
        }
        case DEC_HELD:
        {
            count++;
            if(count >= sec1)
            {
                count = 0;
                value--;
                if(value <= 0)
                {
                    value = 0; 
                }

                LCD_Cursor(1);
                LCD_WriteData(value + '0');
            }

            break;
        }
        case RESET:
        {
            value = 0; 
            LCD_Cursor(1);
            LCD_WriteData(value + '0');

            break;
        }
    }
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(50);
    TimerOn();

    LCD_init();

    // initial lcd display
    LCD_Cursor(1);
    LCD_WriteData('0');
    while (1) 
    {
        tick();

        while(!TimerFlag) {}
        TimerFlag = 0;
    }
    
    return 1;
}

