/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: 
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

#include "../header/io.h"

int main() 
{
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    char value = 2;
    LCD_Cursor(1);
    LCD_WriteData(value + '0');
    LCD_WriteData((value + 1) + '0');
    LCD_ClearScreen();
    while (1) 
    {
//        continue;
    }
    return 1;
}
