/*
 * beeper.c
 *
 * Created: 11.05.2026 10:11:03
 *  Author: Святослав
 */ 

#include "avr/io.h"
#include "beeper.h"


void init_beeper(){
	DDR_BEEPER |= (1 << PIN_BEEPER);
	PORT_BEEPER &= ~(1 << PIN_BEEPER);
}

void beeper_on(){
	PORT_BEEPER |= (1 << PIN_BEEPER);
}

void beeper_off(){
	PORT_BEEPER &= ~(1 << PIN_BEEPER);
}