/*
 * button.h
 *
 * Created: 06.03.2023 13:05:23
 *  Author: slava
 */ 

#ifndef BUTTON_H
#define BUTTON_H

//#define FLAG_BUTTON 1

#define DELAY_ANTIDREBEZG 300
#define TIME_LONG_PRESS_BUTTON (10UL*2000UL) /* Продолжительность нажатия на кнопку, считающееся длительным. 1500 mc */

#define DIRECT_BUTTONS DDRD
#define PORT_BUTTONS PORTD
#define PIN_BUTTONS PIND
#define PIN_BUTTON PORTD3


void init_port_button();
void read_button();
void clicked_button2();

#endif /* BUTTON_H_ */