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
#define PIN_BEEPER PORTC0
#define NUMBER_TAKTS_FOR_PISK 100

void init_beeper();
void beeper_on();
void beeper_off();
void pisk();

#endif /* BEEPER_H_ */