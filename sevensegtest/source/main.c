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
#include <avr/eeprom.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "../header/shiftRegister.h"

#include "../header/io.h"

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00; 
    DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    
    shiftInit();
    LCD_Init();
    
    LCD_Cursor(1);
    LCD_WriteData('a');

    while (1) 
    {

    }

    return 0;
}
