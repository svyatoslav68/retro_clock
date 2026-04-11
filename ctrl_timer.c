//#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "data_to_display.h"
#include "main.h"
#include "RTOS.h"
#include "button.h"
//#include "tasks.h"
#include "ctrl_timer.h"

typedef struct {
	TPTR func;
	size_t current_tik; /* текущий счет */
	size_t num_tiks;
	/* количество счетов для задержки выполнения функции.
	 * если num_tiks == 0, то func выполняется единожды */
} queue_node_t;

typedef struct {
	queue_node_t *nodes;
	size_t size;
} queue_t;

queue_node_t timer_queue[TIMER_QUEUE_SIZE];
queue_t timer_tasks;// = {timer_queue, 0};

queue_node_t timer_task_NULL = {NULL, 0, 0};
	
void swap (queue_node_t *first, queue_node_t *second);
TPTR get_top_task();
queue_node_t pop_task();


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

TPTR get_top_task()
{
	if (!(timer_tasks.size))
	return NULL;
	queue_node_t *top_node = timer_tasks.nodes;
	return top_node->func;
}

queue_node_t less_node(const queue_node_t first, const queue_node_t second)
{
	return first.num_tiks<second.num_tiks?first:second;
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
	*(timer_tasks.nodes + (timer_tasks.size)++) = new_task;
	up(timer_tasks.nodes, timer_tasks.size - 1);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
}

void add_new_task_with_delay(const TPTR task, const uint16_t delay, const uint16_t tiks)
{
	const queue_node_t timer_task = {task, delay, tiks};
	add_new_task(timer_task);
}

void add_task_with_repeat(queue_node_t node)
/* Добавление задачи в очередь задач с последующим добавлением в очередь таймера */
{
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	add_task(node.func);
	add_new_task(node);
	TIMSK = tmp_TIMSK;
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
	//swap(timer_tasks.nodes,(timer_tasks.nodes + TIMER_QUEUE_SIZE - 1));
	down(timer_tasks.nodes, --timer_tasks.size, 0);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
	return result;
}

TPTR pop_func()
{
	queue_node_t temp = pop_task();
	return temp.func;
}

void execute_task()
{
	if(timer_tasks.size == 0)
	return;
	const queue_node_t root_node = *(timer_tasks.nodes);
	/* Запоминаем состояние регистра флагов прерываний */
	uint8_t tmp_TIMSK = TIMSK;
	/* Выключаем прерывание по таймеру, поскольку работаем с очередью задач таймера */
	TIMSK &= ~(1 << TIMER_INTERRUPT_FLAG);
	swap(timer_tasks.nodes,(timer_tasks.nodes + timer_tasks.size - 1));
	down(timer_tasks.nodes, --timer_tasks.size, 0);
	/* Восстанавливаем содержимое регистра флагов прерываний */
	TIMSK = tmp_TIMSK;
	root_node.func();
	//printf("execute func!\nResult = %d\n", root_node.func(root_node.num_tiks));
}
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


void swap_timer_task(queue_node_t *first, queue_node_t *second)
{
	queue_node_t *tmp = first;
	first = second;
	second = tmp;
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

void init_test_timer_queue(void)
/* Инициализация таймера для самотестирования */
{
	//queue_node_t blank_led = {};
#ifdef PINBOARD
	add_new_task_with_delay(display_array, 40, 40);
	add_new_task_with_delay(flash_digiting, 3000, 3000);
	add_new_task_with_delay(reading_encoder, 10, 10);
#endif
	/*
	add_new_task_with_delay(blank_led_board, 250, 250);
	queue_node_t on_test = {on_test_led, 50, 0};
	queue_node_t off_test = {off_test_led, 170, 0};
	add_new_task(on_test);
	add_new_task(off_test);
	*/
	//add_new_task(on_test);	
}

ISR(TIMER0_COMP_vect)
{
	//PORT_LEDS |= (1 << PORT_BLANK_LED);
	/*PORT_LEDS &= ~(1 << PORT_TEST);*/
	queue_node_t *current_timer_task = timer_tasks.nodes;//(queue_node_t *)timer_tasks.nodes;
	queue_node_t node_for_repeat = timer_task_NULL;
	while (current_timer_task < (timer_queue + timer_tasks.size)){ // Проходим по всему списку задач таймера
		if ((current_timer_task->func) && (!(current_timer_task->current_tik))) {// Если стало 0, то добавиить в очередь
			if (current_timer_task->num_tiks) { // Если num_tick, значит это повторяемая задача
				node_for_repeat = pop_task();   // Создать переменную, содержащую такую же задачу
				node_for_repeat.current_tik = node_for_repeat.num_tiks; // Восстановить current_tik
				add_task_with_repeat(node_for_repeat);  // Добавить созданную задачу в список задач таймера
				continue;
				//--current_timer_task;
				//add_new_task(*current_timer_task);
			}
			else {
				add_task(pop_func());  // Извлечь функцию и отправить её в список задач на выполнение
				//PORT_LEDS &= ~(1 << PORT_TEST);
			}
		}
		else {
			--current_timer_task->current_tik; // Уменьшить current_tik
		}
		++current_timer_task;
	}
	//PORT_LEDS &= ~(1 << PORT_BLANK_LED);
	//TCNT0 = VALUE_TCNT0;
}

ISR (INT1_vect)
{
	//PORT_LEDS |= (1 << PORT_TEST);
	/* Отключим прерывание. Включим после срабатывания таймера. */
	GICR &= ~(1 << INT1);
	/* Прерывание от кнопки ставит в очередь процедуру чтения состояния кнопки, 
	которая будет определеять состояние после задержки в 20мс */
	queue_node_t read_button_after_delay = {read_button, DELAY_ANTIDREBEZG, 0};
	add_new_task(read_button_after_delay);
	//PORT_LEDS &= ~(1 << PORT_TEST);
}
