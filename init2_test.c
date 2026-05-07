/*
 * init2_test.c
 *
 * Модуль предназначен для отладки приложения.
 * Кнопка будет запускать ту же процедуры, которая в приложении
 * выполняется по таймеру.
 
 * Created: 18.04.2026 11:03:25
 *  Author: svjat
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "init2_test.h"

void start_timer2(){
	TIFR |= (1 << TOV2);
	TIMSK |= (1 << TOIE2);
	/* Сбрасываем счетчик предделителя */
	SFIOR |= (1 << PSR2);
	/* Запускаем таймер с коэффициентом 1024 */
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);
}

void stop_timer2() {
	TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

ISR(INT0_vect) {
	start_timer2();
}

//ISR(TIMER2_OVF_vect) {
void test() {	
	stop_timer2();
	if (PIND & (1 << PIND2)) {
		PORT_FOR_CONTROL_LED ^= (1 << CONTROL_LED);
	}
	enable_int0();
}

void enable_int0(){
	/* СБрасываем флаг и устанавливаем разрешение по прерыванию */
	GIFR |= (1 << INTF0);
	GICR |= (1 << INT0);
}

void disable_int0(){
	GICR &= ~(1 << INT0);
	GIFR |= (1 << INTF0);
}

void init_test(){
	/* Конфигурация порта для использования светодиода для тестирования */
	DDR_FOR_CONTROL_LED |= (1 << CONTROL_LED);
	/* Конфигурация порта для кнопки INT0 */
	DDRD &= ~(1 << PORTD2);
	PORTD |= (1 << PORTD2);
	/* Прерывание срабатывает по любому фронту */
	MCUCR |= (1 << ISC00);
	MCUCR &= ~(1 << ISC01);
	enable_int0();
}

