/* clock.h
 *
 * Created: 08.03.2026 20:41:43
 * Author: svjat
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#define CLOCK_SELECT_BITS_TIMER1 ((1 << CS12)|(1 << CS11))
#define PORT_CTRL_TIMER PORTC
#define PIN_CTRL_TIMER PINC
#define DIRECT_CTRL_TIMER DDRC
#define PIN_ON_TIMER_MINUTE PORTC6
#define PIN_ON_TIMER_SECUNDA PORTC7

//#define FLAG_READ_ENCODER 2

typedef enum {viewclock,  // отображение часов
			  viewalarm,  // отображение будильника
			  viewtimer,  // отображение таймера
			  setclockminutes,   // установка минут часов
			  setclockhours,  // установка часов часов
			  setalarmminutes,   // установка минут будильника
			  setalarmhours,   // установка часов будильника
			  settimerminutes, // установка часов таймера
			  settimerhours, // установка минут таймера
			  alarm,		// звучание будильника
			  notalarm	// остановка будильника
			  } typemode;

void init_port_timer();
void init_clock();
void save_alarm_to_eeprom();
void save_timer_to_eeprom();
void start_timer1();
void stop_timer1();
void change_minute(int8_t);
void change_hour(int8_t);
void change_alarm_minute(int8_t direct);
void change_alarm_hour(int8_t direct);
void change_timer_minute(int8_t direct);
void change_timer_hour(int8_t direct);
void comp_time_alarm();


#endif /* CLOCK_H_ */
