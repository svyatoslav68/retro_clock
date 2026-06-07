/*
 * beeper.c
 *
 * Created: 11.05.2026 10:11:03
 *  Author: Святослав
 */ 

#include "avr/io.h"
#include "beeper.h"
#include "RTOS.h"
#include "ctrl_timer.h"

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
void pisk(){
	add_task(beeper_on);
	add_new_task_with_delay(beeper_off,NUMBER_TAKTS_FOR_PISK,0);
}
