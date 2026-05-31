/* clock.h
 *
 * Created: 08.03.2026 20:41:43
 * Author: svjat
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#define CLOCK_SELECT_BITS_TIMER1 ((1 << CS12)|(1 << CS11))

#define FLAG_EQUAL 0
#define FLAG_NOTEQUAL 1
#define FLAG_READ_ENCODER 2

typedef enum {viewclock,  // отображение часов
			  viewalarm,  // отображение будильника
			  setclockminutes,   // установка минут часов
			  setclockhours,  // установка часов часов
			  setalarmminutes,   // установка минут будильника
			  setalarmhours,   // установка часов будильника
			  alarm,		// звучание будильника
			  notalarm	// остановка будильника
			  } typemode;

void init_clock();
void save_alarm_to_eeprom();
void start_timer1();
void stop_timer1();
void change_minute(int8_t);
void change_hour(int8_t);
void change_alarm_minute(int8_t direct);
void change_alarm_hour(int8_t direct);
void comp_time_alarm();


#endif /* CLOCK_H_ */
