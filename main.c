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

//extern  int8_t number_flash_digit; // Номер мигающего разряда
volatile typemode  mode;
volatile uint8_t flags = 0x00;

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
	
#ifdef PINBOARD
	init_display();
#endif
	init_task_queue();
	init_timer_queue();
	//number_flash_digit = 0;
	init_test_timer_queue();
	init();
	mode = viewclock;
	init_encoder();
#ifdef DEBUG_INT0
	init_test();
#else 
	start_timer0();
	start_timer1();
#endif    
	/* Replace with your application code */
    while (1) 
    {
		//reading_encoder();
		if (flags & (1 << FLAG_EQUAL)){
			if(mode == viewclock){
				mode = alarm;
			}
		}
		if (flags & (1 << FLAG_NOTEQUAL)){
			if((mode == notalarm) || (mode == alarm)){
				mode = viewclock;
			}
		}
		task_manager();
    }
}

