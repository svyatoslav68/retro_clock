#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "RTOS.h"
#include "ctrl_timer.h"
#include "encoder.h"
//#include "timer_queue.h"


//extern type_timer_task timer_queue[TIMER_QUEUE_SIZE];
volatile TPTR task_queue[TASK_QUEUE_SIZE];

void idle(void){
	// Задача-пустышка. Нужна для сравнения адреса функции-задачи.  
}

void init_task_queue(void){
	for(int i=0; i < TASK_QUEUE_SIZE; ++i){
		task_queue[i] = idle;
	}
}

void task_manager(void) /* ядро диспетчера. Он будет крутиться в главном цикле и вызывать задачи на выполнение */
{
	PORT_TEST |= (1 << ONE_PIN_TEST4);
	/* Объявляем переменную указатель на функцию, в которую будем помещать задачи из очереди */
	TPTR task_for_execute = idle;
	//stop_timer0();
	CLI_M16;
	task_for_execute = task_queue[0]; /* Берем первую задачу из очереди  и выполняем её */
	task_for_execute();
	if (task_for_execute == idle) {
		/* Если это пустышка, то выходим */
		goto end_function;
	}
	else { /* если не пустышка, то сдвигаем все остальные задачи вверх, последнюю делаем пустышкой */
		uint8_t index_dest, index_source;
		for (index_dest = 0,index_source=1; index_dest < TASK_QUEUE_SIZE -1; ++index_dest,++index_source){
			//if (task_queue[index_source]==idle) break;
			task_queue[index_dest] = task_queue[index_source];
		}
		task_queue[index_source]=idle;
	} 
	/* Восстанавливаем бит прерываний */
	end_function:
	//start_timer0();
	SEI_M16;
	PORT_TEST &= ~(1 << ONE_PIN_TEST4);
}

void add_task(TPTR TS)
{
	uint8_t flag_interrupt = 0;
	TPTR *task_for_execute = (TPTR*)task_queue;
	if (SREG & 0b10000000) {
		CLI_M16;
		flag_interrupt = 1;
	}
	while ((task_for_execute < (TPTR *)task_queue + TASK_QUEUE_SIZE) && (*task_for_execute != TS)) {
		if ((*task_for_execute) == idle) {
			*task_for_execute = TS;
			break;
		}
		++task_for_execute;
	}
	if (flag_interrupt) {
		SEI_M16;
	}
}

