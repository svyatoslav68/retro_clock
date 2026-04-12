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
			  setclock,   // установка часов
			  setalarm,   // установка будильника
			  alarm,		// звучание будильника
			  notalarm	// остановка будильника
			  } typemode;

void start_timer1();
void stop_timer1();


#endif /* CLOCK_H_ */
