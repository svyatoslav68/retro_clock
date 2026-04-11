/*
 * clock.c
 *
 * Created: 08.03.2026 20:40:44
 *  Author: svjat
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "data_to_display.h"
#include "clock.h"

volatile uint8_t clock_array[LENGTH_ARRAY] = {7, 30};
//volatile uint8_t byte_minutes = 0x00;
volatile uint8_t alarm_array[LENGTH_ARRAY] = {12, 22};
//volatile uint8_t byte_minutes_alarm = 0x00;

extern uint8_t flags;

void start_timer1(){
	TCNT1 = 0x00; // Сброс счетчика
	TCCR1B |= (1 << WGM12);// | (1 << WGM10); // Установка режима СТС
	OCR1A = 60UL; // Счетчик секундный, поэтому после 60 секунд сброс счетчика
	TIFR = (1 << OCF1A);
	TIMSK |= (1 << OCIE1A); // Разрешение прерывания по достижению значения сравнения
	TCCR1B |= CLOCK_SELECT_BITS_TIMER1;     // Установка счёта с внешнего источника по спаду
}

void stop_timer1(){
	TCCR1B &= ~CLOCK_SELECT_BITS_TIMER1;
}

ISR(TIMER1_COMPA_vect) {
	if (++*(clock_array + 1) == 60) {
		*(clock_array + 1) = 0x00;
		if (++(*clock_array) == 24)	{
			*clock_array = 0x00;
		}
	}
	if ((clock_array[0] == alarm_array[0]) && (alarm_array[1] == clock_array[1])){
		flags |= (1 << FLAG_EQUAL);
		flags &= ~(1 <<FLAG_NOTEQUAL);
	}
	else {
		flags &= ~(1 << FLAG_EQUAL);
		flags |= (1 <<FLAG_NOTEQUAL);
	}
}
