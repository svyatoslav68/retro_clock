/*
 * button.c
 * Здесь будет код, необходимый для обработки нажатия кнопок
 * Created: 06.03.2023 13:04:25
 *  Author: slava
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "main.h"
#include "RTOS.h"
#include "data_to_display.h"
#include "button.h"
//#include "timer_queue.h"
//#include "tasks.h"


//extern volatile type_queue timer_tasks;// = {timer_queue, 0};

void clicked_button2()
{
	/*static uint8_t state = 0x00;
	if (state & (1 << FLAG_BUTTON)){
		//off_test_led();
	}
	else {	
		//on_test_led();
	}
	state ^= (1 << FLAG_BUTTON);*/
	next_flash_digit();
}

void read_button()
{
	/* Включаем внешнее прерывание */
	GIFR |= (1 << INTF1);
	GICR |= (1 << INT1);
	if (PIN_BUTTONS & (1 << PIN_BUTTON)) /* Если кнопка отжата */
	{
		add_task(clicked_button2);
	}
}


