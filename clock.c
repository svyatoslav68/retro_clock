/*
 * clock.c
 *
 * Created: 08.03.2026 20:40:44
 *  Author: svjat
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "beeper.h"

#include "data_to_display.h"
#include "clock.h"

volatile int8_t clock_array[LENGTH_ARRAY] = {7, 30};
//volatile uint8_t byte_minutes = 0x00;
volatile int8_t alarm_array[LENGTH_ARRAY] = {12, 22};
//volatile uint8_t byte_minutes_alarm = 0x00;
extern uint8_t eeprom_alarm[LENGTH_ARRAY];
extern typemode  mode;

extern uint8_t flags;

void init_clock(){
	eeprom_read_block((void *)alarm_array, (void *)eeprom_alarm, LENGTH_ARRAY);
}

void save_alarm_to_eeprom(){
	eeprom_write_block((void *)alarm_array, (void *)eeprom_alarm, LENGTH_ARRAY);
}

void add_minute();

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
	if ((mode != setclockminutes)&&(mode != setclockhours)) {
		add_minute();
		if ((clock_array[0] == alarm_array[0]) && (alarm_array[1] == clock_array[1])){
			flags |= (1 << FLAG_EQUAL);
			flags &= ~(1 <<FLAG_NOTEQUAL);
		}
		else {
			flags &= ~(1 << FLAG_EQUAL);
			flags |= (1 <<FLAG_NOTEQUAL);
		}
	}
}

void add_minute(){
	if (++*(clock_array + 1) == 60) {
		*(clock_array + 1) = 0x00;
		if (++(*clock_array) == 24)	{
			*clock_array = 0x00;
		}
	}
}

void change_minute(int8_t direct){
	*(clock_array + 1) += direct;	
		if (*(clock_array + 1) == 60) {
			*(clock_array + 1) = 0;
		}
		if (*(clock_array +1 ) == -1) {
			*(clock_array + 1) =59;
		}
}

void change_hour(int8_t direct){
	*clock_array += direct;
		if (*clock_array == 24) {
			*clock_array = 0;
		}
		if (*clock_array == -1) {
			*clock_array = 23;
		}
}

void change_alarm_minute(int8_t direct){
	*(alarm_array + 1) += direct;
	if (*(alarm_array + 1) == 60) {
		*(alarm_array + 1) = 0;
	}
	if (*(alarm_array +1 ) == -1) {
		*(alarm_array + 1) =59;
	}
}

void change_alarm_hour(int8_t direct){
	*(alarm_array) += direct;
	if (*(alarm_array) == 24) {
		*(alarm_array) = 0;
	}
	if (*(alarm_array) == -1) {
		*(alarm_array) =23;
	}
}

void comp_time_alarm(){
	if (flags & (1 << FLAG_EQUAL)){
		if(mode == viewclock){
			beeper_on();
			mode = alarm;
		}
	}
	if (flags & (1 << FLAG_NOTEQUAL)){
		if((mode == notalarm) || (mode == alarm)){
			 beeper_off();
			 mode = viewclock;
		}
	}
}