/*	Author: oocho002
 *  Partner(s) Name: Gurparam
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x080;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum states {light0, light1, light2, wait, restart} state;
void tick() {
	switch(state) {
		
		case light0:
		{
			PORTB = 0x01;
			if ((~PINA & 0x01) == 0x01) {
				state = wait;
				break;
			}
			state = light1;
			break;
		}
		case light1:
		{
			PORTB = 0x02;
			if ((~PINA & 0x01) == 0x01) {
                                state = wait;
                                break;
                        }
			state = light2;
                        break;
		}
		case light2:
		{
			PORTB = 0x04
			if ((~PINA & 0x01) == 0x01) {
                                state = wait;
                                break;
                        }
			state = light0;
                        break;
		}
		case wait:
		{
			if ((~PINA & 0x01) == 0x01) {
				state = wait;
				break;
			}
			else if ((~PINA & 0x01) == 0x00) {
                                state = restart;
				break;
                        }
		}
		case restart:
                {
                        if ((~PINA & 0x01) == 0x00) {
                                state = restart;
				break;
                        }
                        else if ((~PINA & 0x01) == 0x01) {
                                state = light0;
				break;
                        }
                }
		default:
		{
			state = light0;
			break;
		}
	}
	switch(state) {
		case light0:
		{
			PORTB = 0x01;
                        break;
		}
                case light1:
		{
			PORTB = 0x02;
                        break;
		}
                
		case light2:
		{
			PORTB = 0x04;
                        break;
		}
		case wait:
                {
                        break;
                }
		case restart:
                {
                        break;
                }
        }
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = light0;
	TimerSet(1000);
	TimerOn();
	while(1) {
		tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
    return 1;
}
