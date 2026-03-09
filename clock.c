/*
 * clock.c
 *
 * Created: 08.03.2026 20:40:44
 *  Author: svjat
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"

volatile uint8_t byte_hours = 0x00;
volatile uint8_t byte_minutes = 0x00;

void start_timer1(){
	TCNT1 = 0x00; // Сброс счетчика
	TCCR1A |= (1 << WGM12) | (1 << WGM10); // Установка режима СТС
	OCR1A = 60UL; // Счетчик секундный, поэтому после 60 секунд сброс счетчика
	TIFR = (1 << OCF1A);
	TIMSK |= (1 << OCIE1A); // Разрешение прерывания по достижению значения сравнения
	TCCR1B |= CLOCK_SELECT_BITS_TIMER1;     // Установка счёта с внешнего источника по спаду
}

void stop_timer1(){
	TCCR1B &= ~CLOCK_SELECT_BITS_TIMER1;
}

ISR(TIMER1_COMPA_vect) {
	if (++byte_minutes == 60) {
		byte_minutes = 0x00;
		if (++byte_hours == 24)	{
			byte_hours = 0x00;
		}
	}
}