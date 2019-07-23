#ifndef TIME_H
#define TIME_H

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // must be cleared to 0 

unsigned long _avr_timer_M = 1; // count starts here -> 0 (1 ms by default)
unsigned long _avr_timer_cntcurr = 0; // current internal count (1 ms ticks)

typedef struct _Task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

void TimerISR()
{
    TimerFlag = 0;
}

void TimerOn()
{
    // AVR timer/counter controller register TCCR1
    TCCR1B = 0x0B;

    // AVR output compare register OCR1A
    OCR1A = 125;

    // AVR timer interrupt mask register
    TIMSK1 = 0x02;

    // Init avr counter
    TCNT1 = 0;

    _avr_timer_cntcurr = _avr_timer_M;

    // global interrupts enabled
    SREG |= 0x80;
}

void TimerOff()
{
    TCCR1B = 0x00;
}

ISR(TIMER1_COMPA_vect)
{
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0)
    {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

#endif
