#ifndef CTRL_TIMER_H
#define CTRL_TIMER_H

//#include <stdint.h>

#define  DELAY_TIMER_MS 10UL /* Время задержки, генерируемой таймером в сотых долях мс */
#define  DIVIDER_0 64UL      /* Предделитель для таймера-счетчика 0 */
#if DIVIDER_0 == 1024UL
#define CLOCK_SELECT_BITS_TIMER0 0x05
#elif DIVIDER_0 == 256UL
#define CLOCK_SELECT_BITS_TIMER0 0x04
#elif DIVIDER_0 == 64UL
#define CLOCK_SELECT_BITS_TIMER0 0x03
#endif

#define VALUE_OCR0 ((F_CPU*DELAY_TIMER_MS)/(DIVIDER_0*100000))
#define VALUE_TCNT0 (256UL-F_CPU/(DIVIDER_0*DELAY_TIMER_MS*10))


struct queue_node_t; 
struct queue_t;


void start_timer0();
void stop_timer0();
//void add_timer_task(type_queue_node task);
//void init_test_timer_queue(void);
void add_new_task_with_delay(const TPTR task, const uint16_t delay, const uint16_t tiks);

#endif // CTRL_TIMER_H
