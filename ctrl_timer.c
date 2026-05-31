//#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "data_to_display.h"
#include "main.h"
#include "RTOS.h"
#include "button.h"
#include "ctrl_timer.h"
#include "init2_test.h"
#include "timer_queue.h"

extern queue_node_t timer_queue[];//[TIMER_QUEUE_SIZE];
extern queue_t timer_tasks;
extern queue_node_t timer_task_NULL;

/*_Bool min_node(const queue_node_t *A, const queue_node_t *B)
{
	return A->current_tik < B->current_tik? 1:0;
}

void heapify(queue_node_t *queue, size_t size, size_t i, _Bool (*cmp)(queue_node_t *, queue_node_t *))
{
	size_t left_node = leftChild(i);
	size_t right_node = rightChild(i);
	size_t current_node = i;
	
}*/


/*void execute_task()
{
	if(timer_tasks.size == 0)
	return;
	const queue_node_t root_node = *(timer_tasks.nodes);
	uint8_t tmp_TIMSK = TIMSK;
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	swap(timer_tasks.nodes,(timer_tasks.nodes + timer_tasks.size - 1));
	down(timer_tasks.nodes, --timer_tasks.size, 0);
	// Восстанавливаем содержимое регистра флагов прерываний 
	TIMSK = tmp_TIMSK;
	root_node.func();
	//printf("execute func!\nResult = %d\n", root_node.func(root_node.num_tiks));
}*/

void init_timer_queue()
{
	for(int i=0; i < TIMER_QUEUE_SIZE; ++i){
		timer_queue[i] = timer_task_NULL;
	}
	timer_tasks.nodes = timer_queue;
	timer_tasks.size = 0;
}

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


							
/*void add_timer_task(queue_node_t task)
{
	CLI_M16;
	for (int i = 0; i < TIMER_QUEUE_SIZE; ++i)
	{
		if(timer_queue[i].timer_task == idle){
			timer_queue[i] = task;
			break;
		}
	}
	SEI_M16;
}*/

/*
void timer_service(void)
{
	queue_node_t task_from_timer_queue;
	for (uint8_t index = 0; index < TIMER_QUEUE_SIZE+1; ++index){
		task_from_timer_queue = timer_queue[index];
		if (!task_from_timer_queue.func)
			continue;
		if (!(--task_from_timer_queue.num_tiks)){
			(task_from_timer_queue.func)();
		}
	}	
}
*/


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
	queue_node_t *current_timer_task = timer_tasks.nodes;//(queue_node_t *)timer_tasks.nodes;
	queue_node_t node_for_repeat = timer_task_NULL;
	while (current_timer_task < (timer_queue + timer_tasks.size)){ // Проходим по всему списку задач таймера
		if ((current_timer_task->func) && (!(current_timer_task->current_tik))) {// Если стало 0, то добавиить в очередь
			if (current_timer_task->num_tiks) { // Если num_tick, значит это повторяемая задача
				node_for_repeat = pop_task();   // Создать переменную, содержащую такую же задачу
				node_for_repeat.current_tik = node_for_repeat.num_tiks; // Восстановить current_tik
				add_task_with_repeat(node_for_repeat);  // Добавить созданную задачу в список задач таймера
				//continue;
			}
			else {
				add_task(pop_func());  // Извлечь функцию и отправить её в список задач на выполнение
			}
		}
		else {
			--current_timer_task->current_tik; // Уменьшить current_tik
		}
		++current_timer_task;
	}
	PORT_TEST &= ~(1 << ONE_PIN_TEST1);
#ifdef DEBUG_INT0
	/* Далее строки для отладки  */
	}
	enable_int0();
#endif
}

ISR (INT1_vect)
{
	/* Отключим прерывание. Включим после срабатывания таймера. */
	GICR &= ~(1 << INT1);
	/* Прерывание от кнопки ставит в очередь процедуру чтения состояния кнопки, 
	которая будет определеять состояние после задержки в 20мс */
	queue_node_t read_button_after_delay = {read_button, DELAY_ANTIDREBEZG, 0};
	add_new_task(read_button_after_delay);
}
