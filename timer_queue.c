/*
 * timer_queue.c
 *
 * Created: 31.05.2026 12:18:18
 *  Author: Святослав
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include "RTOS.h"
#include "data_to_display.h"
#include "clock.h"
#include "timer_queue.h"


queue_node_t timer_queue[TIMER_QUEUE_SIZE];
queue_t timer_tasks;// = {timer_queue, 0};

queue_node_t timer_task_NULL = {NULL, 0, 0};

void swap (queue_node_t *first, queue_node_t *second);
TPTR get_top_task();
queue_node_t pop_task();


void init_timer_queue()
{
	for(int i=0; i < TIMER_QUEUE_SIZE; ++i){
		timer_queue[i] = timer_task_NULL;
	}
	timer_tasks.nodes = timer_queue;
	timer_tasks.size = 0;
}

void init_timer_queue_with_tasks()
{
	const queue_node_t display_task = {display_array, 30, 30};
	const queue_node_t blinking_task = {flash_digiting, 3000, 3000};
	const queue_node_t blink_dot_task = {flash_dot, 5000, 5000};
	const queue_node_t comp_time_task = {comp_time_alarm, 500, 500};
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	init_timer_queue();
	*(timer_tasks.nodes + (timer_tasks.size)++) = display_task;
	up(timer_tasks.nodes, timer_tasks.size - 1);
	*(timer_tasks.nodes + (timer_tasks.size)++) = blinking_task;
	up(timer_tasks.nodes, timer_tasks.size - 1);
	*(timer_tasks.nodes + (timer_tasks.size)++) = blink_dot_task;
	up(timer_tasks.nodes, timer_tasks.size - 1);
	*(timer_tasks.nodes + (timer_tasks.size)++) = comp_time_task;
	up(timer_tasks.nodes, timer_tasks.size - 1);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
}

uint8_t parent(const uint8_t i) {
	if (i > 0)
	return (i - 1) / 2;
	else
	return 0;
}

uint8_t leftChild(const uint8_t i) {
	return 2 * i + 1;
}

uint8_t rightChild(const uint8_t i) {
	return 2 * i + 2;
}

void up(queue_node_t *queue, uint8_t i) {
	while (i != 0 && ((queue+i)->current_tik < (queue+parent(i))->current_tik)) {
		swap(queue+i, queue+parent(i));
		i = parent(i);
	}
}

void down(queue_node_t *queue, const uint8_t size, uint8_t i) {
	while (i < size / 2) {
		int minI = leftChild(i);
		if (rightChild(i) < size && ((queue+rightChild(i))->current_tik < (queue+leftChild(i))->current_tik))
		minI = rightChild(i);
		if ((queue+i)->current_tik <= (queue+minI)->current_tik)
		return;
		swap(queue+i, queue+minI);
		i = minI;
	}
}

TPTR get_top_task()
{
	if (!(timer_tasks.size))
	return NULL;
	queue_node_t *top_node = timer_tasks.nodes;
	return top_node->func;
}

void swap (queue_node_t *first, queue_node_t *second)
{
	queue_node_t tmp = *first;
	*first = *second;
	*second = tmp;
}

void add_new_task(const queue_node_t new_task)
{
	if (timer_tasks.size + 1 == TIMER_QUEUE_SIZE)
	/* обработка ошибки - переполнение очереди */
	return;
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	int task_exist = 0;
	/* Проходим по всей очереди и смотрим, есть ли уже такая задача */
	for (queue_node_t *current_task = timer_tasks.nodes; current_task < timer_tasks.nodes + timer_tasks.size; ++current_task){
		if (current_task == &new_task) {
			task_exist = 1;
			break;
		}
	} 
	/* Задача добавляется, если в очереди такой задачи пока нет */
	if (!task_exist) {
		*(timer_tasks.nodes + (timer_tasks.size)++) = new_task;
	}
	up(timer_tasks.nodes, timer_tasks.size - 1);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
}

void add_new_task_with_delay(const TPTR task, const uint16_t delay, const uint16_t tiks)
{
	const queue_node_t timer_task = {task, delay, tiks};
	add_new_task(timer_task);
}

void add_task_with_repeat(const queue_node_t node)
/* Добавление задачи в очередь задач с последующим добавлением в очередь таймера */
{
	/* Посылаем задачу на выполнение в очередь задач */
	add_task(node.func);
	add_new_task(node);
}

queue_node_t pop_task()
{
	if(timer_tasks.size == 0)
	return timer_task_NULL;
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	queue_node_t result = *timer_tasks.nodes;
	swap(timer_tasks.nodes,(timer_tasks.nodes + timer_tasks.size - 1));
	*(timer_tasks.nodes + timer_tasks.size - 1) = timer_task_NULL;
	//swap(timer_tasks.nodes,(timer_tasks.nodes + TIMER_QUEUE_SIZE - 1));
	down(timer_tasks.nodes, --timer_tasks.size, 0);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
	return result;
}

void delete_task_from_queue(const TPTR task){
	for (queue_node_t *current_task = timer_tasks.nodes; current_task < timer_tasks.nodes + timer_tasks.size; ++current_task){
		if (current_task->func == task) {
			swap(current_task, timer_tasks.nodes + timer_tasks.size - 1);
			*(timer_tasks.nodes + timer_tasks.size - 1) = timer_task_NULL;
			down(current_task, --timer_tasks.size, 0);
			break;
		}
	} 
}

void exec_top_task(){
	/* Запуск задачи с верхушки очереди, если её current_tik == 0,
	 * удаление её из очереди и добавление её в очередь, 
	 * если num_tiks != 0 */
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
 	queue_node_t *top_timer_task = timer_tasks.nodes;
	// Если стало 0, то добавить в очередь
	if (!(top_timer_task->current_tik)) {
		queue_node_t node_for_repeat = pop_task();   // Создать переменную, содержащую такую же задачу
		if (node_for_repeat.func) {
			add_task(node_for_repeat.func);
			if (node_for_repeat.num_tiks) { // Если num_tick, значит это повторяемая задача
				node_for_repeat.current_tik = node_for_repeat.num_tiks; // Восстановить current_tik
				add_new_task(node_for_repeat);  // Добавить созданную задачу в список задач таймера
			}
		}
	}
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
}

/*TPTR pop_func()
{
	queue_node_t temp = pop_task();
	return temp.func;
}*/
