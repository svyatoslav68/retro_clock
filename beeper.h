/*
 * beeper.h
 *
 * Created: 11.05.2026 10:10:07
 *  Author: Святослав
 */ 


#ifndef BEEPER_H_
#define BEEPER_H_


#define PORT_BEEPER PORTC
#define DDR_BEEPER DDRC

void beeper_on();
void beeper_off();

#endif /* BEEPER_H_ */