/*
 * retro_clock.c
 *
 * Created: 08.03.2026 6:17:31
 * Author : svjat
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "data_to_display.h"
#include "RTOS.h"
#include "ctrl_timer.h"
#include "clock.h"

//extern  int8_t number_flash_digit; // Номер мигающего разряда

void init() {
	/* Установка параметров внешних прерываний */
	MCUCR = (0 << ISC11)|(1 << ISC10); // Срабатывание по каждому фронту
	GIFR |= (1 << INTF1);
	GICR = (1 << INT1);
	SEI_M16;
}

int main(void)
{
	init();
#ifdef PINBOARD
	init_display();
#endif
	init_task_queue();
	init_timer_queue();
	//number_flash_digit = 0;
	init_test_timer_queue();
	start_timer0();
	start_timer1();
    /* Replace with your application code */
    while (1) 
    {
		task_manager();
    }
}

