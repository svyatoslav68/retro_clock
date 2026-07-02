/*
 * retro_clock.c
 *
 * Created: 08.03.2026 6:17:31
 * Author : svjat
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "main.h"
#include "data_to_display.h"
#include "RTOS.h"
#include "ctrl_timer.h"
#include "clock.h"
#include "encoder.h"
#include "init2_test.h"
#include "button.h"
#include "beeper.h"

//extern  int8_t number_flash_digit; // Номер мигающего разряда
volatile typemode  mode;


void init() {
	/* Установка портов ввода/вывода */
	//DIRECT_LEDS_PINBOARD = 0x0F; /* Младшие разряды на ввод, старшие на вывод */
	//PORT_LEDS_PINBOARD = 0x0F; /* Подтягиваем вводные разряды порта */
	/* Установка параметров внешних прерываний */
	MCUCR |= (1 << ISC10); // Срабатывание по каждому фронту
	MCUCR &= ~(1 << ISC11);
	GIFR |= (1 << INTF1);
	GICR |= (1 << INT1);
	SEI_M16;
}

int main(void)
{
	
#if defined(PINBOARD) || defined(M16BOARD)
	init_display();
#endif
	init_task_queue();
	init_timer_queue();
	//number_flash_digit = 0;
	//init_test_timer_queue();
	init();
	init_clock();
	init_beeper();
	init_port_button();
	init_port_timer();
	mode = viewclock;
	init_encoder();
#ifdef DEBUG_INT0
	init_test();
#else 
	start_timer0();
	start_timer1();
#endif    
#if defined(PINBOARD) || defined(M16BOARD)
	add_new_task_with_delay(display_array, 40, 40);
	add_new_task_with_delay(flash_digiting, 3000, 3000);
	add_new_task_with_delay(flash_dot, 5000, 5000);
	add_new_task_with_delay(reading_encoder, 2, 2);
	add_new_task_with_delay(comp_time_alarm, 500, 500);
#endif
	/* Replace with your application code */
    while (1) 
    {
		//reading_encoder();
		task_manager();
    }
}

