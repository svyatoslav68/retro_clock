#include <avr/io.h>
#include <avr/eeprom.h>
#include "RTOS.h"
#include "button.h"
//#include "timer_queue.h"
#include "data_to_display.h"

volatile uint8_t data_for_display[NUMBER_DIGIT] = {1,2,3};
uint8_t mask_digits = 0x00; // Маска, биты соответстующие отображаемым разрядам равны "1"
volatile uint8_t flash_digit = 0;
volatile int8_t number_flash_digit = -1; // Номер мигающего разряда

uint8_t EEMEM digits[] = {IND_ZERO, IND_ONE, IND_TWO, IND_THREE, IND_FOUR, IND_FIVE, IND_SIX, IND_SEVEN, IND_EIGHT, IND_NINE, IND_DOT, IND_MINUS};

void next_flash_digit(void)
{
	if (number_flash_digit == -1){
		number_flash_digit = 0;
	}
	else {
		number_flash_digit = (++number_flash_digit)%NUMBER_DIGIT;
	}
}

void stop_flashing(){
	number_flash_digit = -1;
}

void init_display(void)
{
	DIRECT_DISPLAY = 0xFF;
	for (int i = 0; i < NUMBER_DIGIT; ++i){
		//mask_digits |= (1 << i);
		mask_digits = flash_digit | (1 << (i + PORT_DIGIT_0));
		flash_digit = mask_digits;
	}
	DIRECT_DIGITS = flash_digit;
}	

void flash_digiting(void)
{
	if (number_flash_digit == -1){
		init_display();
	}
	else {
		flash_digit = (flash_digit == mask_digits)?mask_digits ^ (1 << (number_flash_digit + PORT_DIGIT_0)):mask_digits;
	}
}

void display_array(void)
{
	static uint8_t number_digit = 0; // Номер отображаемого разряда
	uint8_t byte_data = 0x00;
	byte_data = eeprom_read_byte(digits + *(data_for_display + (NUMBER_DIGIT-number_digit-1))); // Считываем из ПЗУ байт соответсвующий цифре
	PORT_DISPLAY = byte_data;
	PORT_DIGITS &= ~mask_digits;
	PORT_DIGITS |= ((1 << (number_digit + PORT_DIGIT_0)) & flash_digit);	
	if (++number_digit == NUMBER_DIGIT)
		number_digit = 0;
	//add_new_task_with_delay(&timer_tasks, display_array, 50);
}

