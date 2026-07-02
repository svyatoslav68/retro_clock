//#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "data_to_display.h"
#include "main.h"
#include "RTOS.h"
#include "timer_queue.h"
//#include "button.h"
#include "ctrl_timer.h"
#include "init2_test.h"

extern queue_node_t timer_queue[];//[TIMER_QUEUE_SIZE];
extern queue_t timer_tasks;
extern queue_node_t timer_task_NULL;



void start_timer0()
/* Установка таймера 0 для формирования прерывания 1мс */
{
	//SFIOR = (1 << PSR10);
	TCNT0 = 0x00;
	OCR0 = VALUE_OCR0;
	TCCR0 = (1 << WGM01);
	TIFR = (1 << OCF0);
	TIMSK = (1 << TIMER_INTERRUPT_FLAG);
	TCCR0 |= CLOCK_SELECT_BITS_TIMER0;
}

void stop_timer0()
{
	TCCR0 &= ~CLOCK_SELECT_BITS_TIMER0;
}

#ifdef DEBUG_INT0
ISR(TIMER2_OVF_vect) {
	/* Далее строки для отладки  */
	stop_timer2();
	disable_int0();
	if (PIND & (1 << PIND2)) {
		PORT_FOR_CONTROL_LED ^= (1 << CONTROL_LED);
#else
ISR(TIMER0_COMP_vect) {
#endif
	PORT_TEST |= (1 << ONE_PIN_TEST1);
	/* Далее боевая функция */
	/* Проходим по всему списку задач таймера 
	 * и уменьшить current_tik на единицу */
	queue_node_t *current_timer_task = timer_tasks.nodes; /* Встаем на верхушку дерева */
	while (current_timer_task < (timer_queue + timer_tasks.size)){
		if ((current_timer_task->current_tik) > 0)
		--(current_timer_task->current_tik); // Уменьшить current_tik
		++current_timer_task;
	}
	PORT_TEST &= ~(1 << ONE_PIN_TEST1);
#ifdef DEBUG_INT0
	/* Далее строки для отладки  */
	}
	enable_int0();
#endif
}



