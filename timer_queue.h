/*
 * timer_queue.h
 *
 * Created: 31.05.2026 12:18:45
 *  Author: Святослав
 */ 


#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#define TIMER_QUEUE_SIZE 20
/* Флаг разрешения прерывания, по которому работает очередь задач таймера */
#define TIMER_INTERRUPT_FLAG OCIE0

typedef struct {
	TPTR func;
	int16_t current_tik; /* текущий счет */
	int16_t num_tiks;
	/* количество счетов для задержки выполнения функции.
	 * если num_tiks == 0, то func выполняется единожды */
} queue_node_t;

typedef struct {
	queue_node_t *nodes;
	size_t size;
} queue_t;

queue_node_t pop_task();
void add_new_task(const queue_node_t new_task);
void add_task_with_repeat(const queue_node_t node);
//TPTR pop_func();
void exec_top_task();

#endif /* TIMER_QUEUE_H_ */