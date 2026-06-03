#include <avr/io.h>
#include <avr/eeprom.h>
#include <stddef.h>
#include "RTOS.h"
#include "button.h"
#include "encoder.h"
#include "clock.h"
#include "data_to_display.h"

volatile uint8_t data_for_display[LENGTH_ARRAY] = {1,45};
extern int8_t clock_array[LENGTH_ARRAY];
extern int8_t alarm_array[LENGTH_ARRAY];
extern typemode  mode;
uint8_t mask_digits = 0x00; // Маска, биты соответстующие отображаемым разрядам равны "1"
volatile uint8_t flash_digit = 0;
volatile int8_t number_flash_digit = -1; // Номер мигающего разряда
volatile uint8_t displayed_dot = 0;

extern uint8_t digits[];
int8_t displayed_number = 0x00;

void next_flash_digit(void)
{
		if (++number_flash_digit == LENGTH_ARRAY)
			number_flash_digit = 0;		
}

void stop_flashing(){
	number_flash_digit = -1;
}

void init_display(void)
{
	DIRECT_DISPLAY = 0xFF;
	for (int i = 0; i < NUMBER_DIGIT; ++i){
		//mask_digits |= (1 << i);
		mask_digits = mask_digits | (1 << (i + PORT_DIGIT_0));
	}
	flash_digit = mask_digits;
	DIRECT_DIGITS = mask_digits;
}	

void flash_digiting(void)
{
	if (number_flash_digit == -1){
		init_display();
	}
	else {
		if (flash_digit == mask_digits) {
			for (register uint8_t i = 0; i < DIGIT_FOR_NUMBER; ++i) {
				flash_digit = flash_digit ^ \
				 (1 << (number_flash_digit*DIGIT_FOR_NUMBER + i + PORT_DIGIT_0));
			}
		}
		else {
			flash_digit = mask_digits;
		}
	}
}

void display_array(void)
{
	static uint8_t number_digit = 0; // Номер отображаемого разряда
	uint8_t byte_data = 0x00;
	displayed_number = 0x00;
	PORT_TEST |= (1 << ONE_PIN_TEST3);
	switch (mode){
	 case viewclock:
	 case setclockminutes:
	 case setclockhours:
	 case alarm:
	 case notalarm:
		displayed_number = *(clock_array + (LENGTH_ARRAY-number_digit/DIGIT_FOR_NUMBER-1));
		break;
	 case viewalarm:
	 case setalarmminutes:
	 case setalarmhours:
		displayed_number = *(alarm_array + (LENGTH_ARRAY-number_digit/DIGIT_FOR_NUMBER-1));
		break;
	 default:
		;
	}
	if (DIGIT_FOR_NUMBER > 1) {
		byte_data = eeprom_read_byte(digits + (number_digit%2)?displayed_number/10:displayed_number%10); // Считываем из ПЗУ байт соответсвующий цифре
	}
	else {
		byte_data = eeprom_read_byte(digits + displayed_number);
	}
#ifdef COMMON_CATOD
	PORT_DISPLAY = (!displayed_dot | !((mode == viewclock)|(mode == alarm)|(mode == notalarm)))?(byte_data | IND_DOT):byte_data;
#endif /* COMMON_CATOD */
#ifdef COMMON_ANOD
	PORT_DISPLAY = (!displayed_dot | !((mode == viewclock)|(mode == alarm)|(mode == notalarm)))?(byte_data & IND_DOT):byte_data;
#endif /* COMMON_CATOD */
	PORT_DIGITS &= ~mask_digits;
	PORT_DIGITS |= ((1 << (number_digit + PORT_DIGIT_0)) & flash_digit);	
	if (++number_digit == NUMBER_DIGIT)
		number_digit = 0;
	//add_new_task_with_delay(&timer_tasks, display_array, 50);
	PORT_TEST &= ~(1 << ONE_PIN_TEST3);
}

void flash_dot (){
	displayed_dot = (displayed_dot == 0)?1:0;
};

