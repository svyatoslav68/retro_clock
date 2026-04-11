/*
 * button.c
 * Здесь будет код, необходимый для обработки нажатия кнопок
 * Created: 06.03.2023 13:04:25
 *  Author: slava
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include "main.h"
#include "RTOS.h"
#include "ctrl_timer.h"
#include "data_to_display.h"
#include "button.h"
#include "clock.h"
//#include "timer_queue.h"
//#include "tasks.h"


//extern volatile type_queue timer_tasks;// = {timer_queue, 0};
#define FLAG_BUTTON_PRESSED 0
#define FLAG_LONGTIME_BUTTON 1

static uint8_t flags_button = 0x00;
extern typemode  mode;

void clicked_button2()
/* Функция выполняется при click'е на кнопку */
{
	switch (mode)
	{
		case viewclock:
		 mode = viewalarm;
		 break;
		case viewalarm:
		 mode = viewclock;
		 break;
		case setclock:
			next_flash_digit();
			break;
		case setalarm:
			next_flash_digit();
			break;
		case alarm:
		 mode = notalarm;
		 break;
		default:
		 break;
	}
	//next_flash_digit();
}

void long_pressed_button()
/* Функция выполняется в результате длинного нажатия на кнопку */
{
	//PORT_LEDS_PINBOARD ^= (1 << PORT_TEST);
    //stop_flashing();
	switch(mode) {
		case viewclock:
		case notalarm:
			mode = setclock;
			next_flash_digit();
			break;
		case viewalarm:
			mode = setalarm;
			next_flash_digit();
			break;
		case setclock:
			mode = viewclock;
			stop_flashing();
			break;
		case setalarm:
			mode = viewalarm;
			stop_flashing();
			break;
		case alarm:
			mode = notalarm;
			break;
	}
}

void definition_longtime()
/* Функция выполняется после задержки на длинное  нажатие.
 * Если кнопка еще числится нажатой, то соответствующая функция 
 * добавляется в очередь задач */
{
    if (flags_button & (1 << FLAG_BUTTON_PRESSED)) {
		flags_button |= (1 << FLAG_LONGTIME_BUTTON);
        add_task(long_pressed_button);
    }
}

void read_button()
/* Функция выполняется после задержки, направленной на отсечение
 * дребезга. Если определяется, что произошел click, т.е. нажатие
 * и быстрое отпускание кнопки, то соответствующая функция добавляется
 * в очередь задач */
{
	/* Включаем внешнее прерывание */
	GIFR |= (1 << INTF1);
	GICR |= (1 << INT1);
	if (PIN_BUTTONS & (1 << PIN_BUTTON)) /* Если кнопка отжата */
	{
		/* Сбрасываем флаг  нажатия */
		flags_button &= ~(1 << FLAG_BUTTON_PRESSED);
		if (!(flags_button & (1 << FLAG_LONGTIME_BUTTON))) { /* Если не зафиксировано долгое  нажатие */
            add_task(clicked_button2);
        }
		/* В любом случае, при отпускании кнопки сбрасываем флаг длинного нажатия*/
		flags_button &= ~(1 << FLAG_LONGTIME_BUTTON); 
	}
    else {
		flags_button |= (1 << FLAG_BUTTON_PRESSED);
		add_new_task_with_delay(definition_longtime, TIME_LONG_PRESS_BUTTON, 0);
    }
}



