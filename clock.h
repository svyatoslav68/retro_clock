/*
 * clock.h
 *
 * Created: 08.03.2026 20:41:43
 *  Author: svjat
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#define CLOCK_SELECT_BITS_TIMER1 ((1 << CS12)|(1 << CS11))

void start_timer1();
void stop_timer1();


#endif /* CLOCK_H_ */