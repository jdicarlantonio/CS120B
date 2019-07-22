/*	Author: Joseph DiCarlantonio
 *  Partner(s) Name: N/A
 *	Lab Section:
 *	Assignment: Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "../header/time.h"
#include "../header/playerMovement.h"
#include "../header/playerShots.h"
#include "../header/game.h"

#define NUM_TASKS 4

const unsigned long tasksPeriod = 50;

Task playerMovement;
Task playerShoot;
Task updateShots;
Task gameTask;
Task* tasks[NUM_TASKS] = {
    &playerMovement,
    &playerShoot,
    &updateShots,
    &gameTask
};

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
    DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0xFF; PORTC = 0x00;

    LCD_Init();

    TimerSet(tasksPeriod);
    TimerOn();

    LCD_Custom(PL_STATIONARY, playerStationary);
    LCD_Custom(MONSTER, monsterNPC); 
    LCD_Custom(PL_KNEEL, playerKneel); 
    LCD_Custom(FRIENDLY, friendlyNPC); 
    LCD_Cursor(ROW1_BEGIN);
    LCD_WriteData(PL_STATIONARY);

    playerMovement.state = WAIT;
    playerMovement.period = 50;
    playerMovement.elapsedTime = playerMovement.period;
    playerMovement.TickFct = &playerMovementTick;
    
    playerShoot.state = WAIT_SHOT;
    playerShoot.period = 50;
    playerShoot.elapsedTime = playerShoot.period;
    playerShoot.TickFct = &playerShootTick;
    
    updateShots.state = UPDATE_SHOTS;
    updateShots.period = 100;
    updateShots.elapsedTime = updateShots.period;
    updateShots.TickFct = &updateShotsTick;
    
    gameTask.state = SC_START;
    gameTask.period = 250;
    gameTask.elapsedTime = gameTask.period;
    gameTask.TickFct = &gameTick;
    
    while (1) 
    {
    }

    return 0;
}
