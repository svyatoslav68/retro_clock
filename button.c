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
#include "encoder.h"
#include "button.h"
#include "clock.h"
#include "beeper.h"
#include "timer_queue.h"
#include "test.h"
//#include "tasks.h"


//extern volatile type_queue timer_tasks;// = {timer_queue, 0};
#define FLAG_BUTTON_PRESSED 0
#define FLAG_LONGTIME_BUTTON 1

static uint8_t flags_button = 0x00;
extern typemode  mode;


ISR (INT1_vect)
{
	/* Отключим прерывание. Включим после срабатывания таймера. */
	GICR &= ~(1 << INT1);
	PORT_TEST |= (1 << ONE_PIN_TEST2);
	/* Прерывание от кнопки ставит в очередь процедуру чтения состояния кнопки, 
	которая будет определеять состояние после задержки в 20мс */
	queue_node_t read_button_after_delay = {read_button, DELAY_ANTIDREBEZG, 0};
	add_new_task(read_button_after_delay);
}

void init_port_button(){
	DIRECT_BUTTONS &= ~(1 << PIN_BUTTON);
	PORT_BUTTONS |= (1 << PIN_BUTTON);
}

void clicked_button2()
/* Функция выполняется при click'е на кнопку */
{
	switch (mode)
	{
		case viewclock:
		case notalarm:
		 mode = viewalarm;
		 break;
		case viewalarm:
		 mode = viewtimer;
		 break;
		case viewtimer:
		 beeper_off();
		 mode = viewclock;
		 init_clock();
		 break;
		case setclockminutes:
			mode = setclockhours;
			next_flash_digit();
			break;
		case setclockhours:
			mode = setclockminutes;
			next_flash_digit();
			break;
		case setalarmminutes:
			mode = setalarmhours;
			next_flash_digit();
			break;
		case setalarmhours:
			mode = setalarmminutes;
			next_flash_digit();
			break;
		case settimerminutes:
			mode = settimerhours;
			next_flash_digit();
			break;
		case settimerhours:
			mode = settimerminutes;
			next_flash_digit();
			break;	
		case alarm:
		    mode = notalarm;
			beeper_off();
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
			mode = setclockminutes;
			//stop_timer1();
			enable_encoder();
			next_flash_digit();
			break;
		case viewalarm:
			mode = setalarmminutes;
			//start_timer1();
			enable_encoder();
			next_flash_digit();
			break;
		case viewtimer:
			mode = settimerminutes;
			enable_encoder();
			next_flash_digit();
			break;
		case setclockminutes:
		case setclockhours:
			mode = viewclock;
			disable_encoder();
			stop_flashing();
			break;
		case setalarmminutes:
		case setalarmhours:
			mode = viewalarm;
			disable_encoder();
			save_alarm_to_eeprom();
			stop_flashing();
			break;
		case settimerminutes:
		case settimerhours:
			mode = viewtimer;
			disable_encoder();
			save_timer_to_eeprom();
			stop_flashing();
			break;
		case alarm:
			/*mode = notalarm;
			beeper_off();
			break;*/
		default:
			;	
	}
}

void definition_longtime()
/* Функция выполняется после задержки на длинное  нажатие.
 * Если кнопка еще числится нажатой, то соответствующая функция 
 * добавляется в очередь задач */
{
    if (flags_button & (1 << FLAG_BUTTON_PRESSED)) {
		flags_button |= (1 << FLAG_LONGTIME_BUTTON);
		add_task(pisk);
        add_task(long_pressed_button);
    }
	PORT_TEST &= ~(1 << ONE_PIN_TEST3);
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
		delete_task_from_queue(definition_longtime);
		flags_button &= ~(1 << FLAG_LONGTIME_BUTTON); 
	}
    else {
		flags_button |= (1 << FLAG_BUTTON_PRESSED);
		PORT_TEST &= ~(1 << ONE_PIN_TEST3);
		PORT_TEST |= (1 << ONE_PIN_TEST3);
		add_new_task_with_delay(definition_longtime, TIME_LONG_PRESS_BUTTON, 0);
    }
	PORT_TEST &= ~(1 << ONE_PIN_TEST2);
}



