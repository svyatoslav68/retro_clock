#ifndef RTOS_H
#define RTOS_H

#define TASK_QUEUE_SIZE 10

typedef void (*TPTR)(void);

void idle(void);

void init_task_queue(void);
void task_manager(void);
void timer_service(void);

void add_task(TPTR TS);

#endif // RTOS_H
