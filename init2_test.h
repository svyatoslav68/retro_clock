/*
 * int2_test.h
 *
 * Created: 18.04.2026 10:59:48
 *  Author: svjat
 */ 


#ifndef INT2_TEST_H_
#define INT2_TEST_H_

#define DDR_FOR_CONTROL_LED DDRD
#define PORT_FOR_CONTROL_LED PORTD
#define CONTROL_LED PORTD7

void init_test();
void start_timer2();
void stop_timer2();
void enable_int0();
void disable_int0();




#endif /* INT2_TEST_H_ */