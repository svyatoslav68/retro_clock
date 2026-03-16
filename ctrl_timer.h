#ifndef CTRL_TIMER_H
#define CTRL_TIMER_H

//#include <stdint.h>
#define TIMER_QUEUE_SIZE 10

#define  DELAY_TIMER_MS 1UL /* Время задержки, генерируемой таймером в мс */         
#define  DIVIDER_0 256UL   /* Предделитель для таймера-счетчика 0 */
#if DIVIDER_0 == 1024UL
#define CLOCK_SELECT_BITS_TIMER0 0x05
#elif DIVIDER_0 == 256UL
#define CLOCK_SELECT_BITS_TIMER0 0x04
#elif DIVIDER_0 == 64UL
#define CLOCK_SELECT_BITS_TIMER0 0x03
#endif

#define VALUE_OCR0 (F_CPU/(DIVIDER_0*DELAY_TIMER_MS*1000))
#define VALUE_TCNT0 (256UL-F_CPU/(DIVIDER_0*DELAY_TIMER_MS*1000))

/* Флаг разрешения прерывания, по которому работает очередь задач таймера */
#define TIMER_INTERRUPT_FLAG OCIE0

//extern type_queue timer_tasks;
     
/*typedef struct {
	TPTR timer_task; // Задача от таймера 
	uint16_t tiks; // Количество тиков до выполнения 
} type_timer_task;*/

struct queue_node_t; 
struct queue_t;

void init_timer_queue();
void start_timer0();
void stop_timer0();
//void add_timer_task(type_queue_node task);
void init_test_timer_queue(void);
void add_new_task_with_delay(const TPTR task, const size_t delay, const size_t tiks);

#endif // CTRL_TIMER_H
