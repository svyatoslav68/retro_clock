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

#define FLAG_ALARM 0
#define FLAG_NOTALARM 1
#define FLAG_TIMER 2
#define FLAG_NOTTIMER 3

volatile int8_t clock_array[LENGTH_ARRAY] = {7, 30};
//volatile uint8_t byte_minutes = 0x00;
volatile int8_t alarm_array[LENGTH_ARRAY] = {12, 22};
//volatile uint8_t byte_minutes_alarm = 0x00;
volatile int8_t timer_array[LENGTH_ARRAY] = {0, 10};
extern uint8_t eeprom_alarm[LENGTH_ARRAY];
extern uint8_t eeprom_timer[LENGTH_ARRAY];

extern typemode  mode;

volatile uint8_t flags = 0x00;

void init_port_timer(){
	DIRECT_CTRL_TIMER &= ~(1 << PIN_ON_TIMER);
	PORT_CTRL_TIMER |= (1 << PIN_ON_TIMER);
}

void init_clock(){
	eeprom_read_block((void *)alarm_array, (void *)eeprom_alarm, LENGTH_ARRAY);
	eeprom_read_block((void *)timer_array, (void *)eeprom_timer, LENGTH_ARRAY);
}

void save_alarm_to_eeprom(){
	eeprom_write_block((void *)alarm_array, (void *)eeprom_alarm, LENGTH_ARRAY);
}

void save_timer_to_eeprom(){
	eeprom_write_block((void *)timer_array, (void *)eeprom_timer, LENGTH_ARRAY);
}

//void inc_clock_minute();

void start_timer1(){
	TCNT1 = 0x00; // Сброс счетчика
	TCCR1B |= (1 << WGM12);// | (1 << WGM10); // Установка режима СТС
	OCR1A = 60UL; // Счетчик секундный, поэтому после 60 секунд сброс счетчика
    OCR1B = 1UL;
	TIFR = (1 << OCF1A)|(1 << OCF1B);
	TIMSK |= (1 << OCIE1A)|(1 << OCIE1B); // Разрешение прерывания по достижению значения сравнения
	TCCR1B |= CLOCK_SELECT_BITS_TIMER1;     // Установка счёта с внешнего источника по спаду
}

void stop_timer1(){
	TCCR1B &= ~CLOCK_SELECT_BITS_TIMER1;
}

/*void inc_clock_minute(){
	if (++*(clock_array + 1) == 60) {
		*(clock_array + 1) = 0x00;
		if (++(*clock_array) == 24)	{
			*clock_array = 0x00;
		}
	}
}*/

/*void dec_timer_minute(){
	if (--*(timer_array + 1) == -1) {
		*(timer_array + 1) = 59;
		if (--(*timer_array) == -1)	{
			*timer_array = 0x00;
		}
	}
}*/

ISR(TIMER1_COMPA_vect) {
	if ((mode != setalarmminutes)&&(mode != setalarmhours)&&(mode != setclockminutes)&&(mode != setclockhours)) {
		//inc_clock_minute();
		if (++*(clock_array + 1) == 60) {
			*(clock_array + 1) = 0x00;
			if (++(*clock_array) == 24)	{
				*clock_array = 0x00;
			}
		}
		/* ---------------------------- */
		if ((clock_array[0] == alarm_array[0]) && (alarm_array[1] == clock_array[1])){
			flags |= (1 << FLAG_ALARM);
			flags &= ~(1 <<FLAG_NOTALARM);
		}
		else {
			flags &= ~(1 << FLAG_ALARM);
			flags |= (1 <<FLAG_NOTALARM);
		}
	}
	if ((mode != settimerminutes)&&(mode != settimerhours)&&(mode != alarm) && (mode != notalarm) && !(PIN_CTRL_TIMER & (1 << PIN_ON_TIMER_MINUTE))) {
		/* dec_timer_minute(); */
		if (--*(timer_array + 1) == -1) {
			*(timer_array + 1) = 59;
			if (--(*timer_array) == -1)	{
				*timer_array = 0x00;
			}
		}
		/* -------------------------------*/
		if ((timer_array[0] == 0) && (timer_array[1] == 0)){
			flags |= (1 << flag_timer);
			flags &= ~(1 <<flag_nottimer);
		}
	}
}

ISR(TIMER1_COMPB_vect) {
}
	if ((mode != settimerminutes)&&(mode != settimerhours)&&(mode != alarm) && (mode != notalarm) && !(PIN_CTRL_TIMER & (1 << PIN_ON_TIMER_SECUNDA))) {
		if (--*(timer_array + 1) == -1) {
			*(timer_array + 1) = 59;
			if (--(*timer_array) == -1)	{
				*timer_array = 0x00;
			}
		}
		/* -------------------------------*/
		if ((timer_array[0] == 0) && (timer_array[1] == 0)){
			flags |= (1 << flag_timer);
			flags &= ~(1 <<flag_nottimer);
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

void change_timer_minute(int8_t direct){
	*(timer_array + 1) += direct;
	if (*(timer_array + 1) == 60) {
		*(timer_array + 1) = 0;
	}
	if (*(timer_array +1 ) == -1) {
		*(timer_array + 1) =59;
	}
}

void change_timer_hour(int8_t direct){
	*(timer_array) += direct;
	if (*(timer_array) == 24) {
		*(timer_array) = 0;
	}
	if (*(timer_array) == -1) {
		*(timer_array) =23;
	}
}
void comp_time_alarm(){
	if (flags & (1 << FLAG_ALARM)){
		if(mode == viewclock){
			beeper_on();
			mode = alarm;
		}
	}
	if (flags & (1 << FLAG_NOTALARM)){
		if((mode == notalarm) || (mode == alarm)){
			 beeper_off();
			 mode = viewclock;
		}
	}
	if (flags & (1 << FLAG_TIMER)){
		beeper_on();
		flags &= ~(1 << FLAG_TIMER);
		mode = viewtimer;
	}
}
