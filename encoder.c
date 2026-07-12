#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

//#include "display.h"
#include "clock.h"
#include "RTOS.h"
#include "timer_queue.h"
#include "ctrl_timer.h"
#include "encoder.h"

extern typemode  mode;
int8_t count_encoder = 0; /* счетчик, который накручен енкодером */
extern int8_t displayed_number;

void init_encoder(){
	DDR_ENCODER &= ~(1 << ENCODER_CHANNEL_A) | (1 << ENCODER_CHANNEL_B);
	DDR_TEST |= (1 << ONE_PIN_TEST1)|(1 << ONE_PIN_TEST2)|(1 << ONE_PIN_TEST3)|(1 << ONE_PIN_TEST4);;
	PORT_ENCODER |= (1 << ENCODER_CHANNEL_A) | (1 << ENCODER_CHANNEL_B);
}

void enable_encoder(){
	add_new_task_with_delay(reading_encoder,2,2);
}

void disable_encoder(){
	//init_timer_queue_with_tasks();
	//delete_task_from_queue(reading_encoder);
}

void reading_encoder(){
	#define NUMBER_PAIRS_IN_BYTE 4 /* Количество пар бит в байте */
	#define FLAG_IS_CHANGE 7
	static uint8_t flags_encoder = 0x00;
	static uint8_t prev_pair_bits = 0x03;
	static uint8_t equal_repeats = 0;     /* Количество повторов принятой комбинации двух бит */
	static uint8_t encoder_byte = 0x00;   /* Байт, состоящих из четырех пар принятых бит */
	register uint8_t current_pair_bits = 0;
	PORT_TEST |= (1 << ONE_PIN_TEST2);
	if ((mode == setalarmminutes) || (mode == setalarmhours) || (mode == setclockminutes) || (mode == setclockhours)
		|| (mode == settimerminutes) || (mode == settimerhours)){
		//stop_timer1();
		current_pair_bits = 
		  ((PIN_ENCODER & (1 << ENCODER_CHANNEL_A)) >> ENCODER_CHANNEL_A) | (((PIN_ENCODER & (1 << ENCODER_CHANNEL_B)) >> ENCODER_CHANNEL_B) << 1);
		if (current_pair_bits == prev_pair_bits) {
			++equal_repeats;
		}
		else {
			flags_encoder |= (1 << FLAG_IS_CHANGE);
			equal_repeats = 0;
		}
		prev_pair_bits = current_pair_bits;
		if ((equal_repeats == NUMBER_RIGHT_VALUE) && (flags_encoder & (1 << FLAG_IS_CHANGE))){
			flags_encoder &= ~(1 << FLAG_IS_CHANGE);
			++flags_encoder;
			encoder_byte = (encoder_byte << 2);
			encoder_byte |= current_pair_bits;
			if ((flags_encoder & 0x07) == NUMBER_PAIRS_IN_BYTE){
				int8_t direct_change = 0;
				switch (encoder_byte) {
					case 0x4b:
					case 0x2c:
					case 0xb4:
					case 0xc2:
					 direct_change = -1;
					 break;
					case 0x1e:
					case 0x78:
					case 0xe1:
					case 0x87:
					 direct_change = 1;
					 break;
					default:
					 direct_change = 0;
					;
				}
				switch (mode) {
					case setclockminutes:
						change_minute(direct_change);
						break;
					case setclockhours:
						change_hour(direct_change);
						break;
					case setalarmminutes:
						change_alarm_minute(direct_change);
						break;
					case setalarmhours:
						change_alarm_hour(direct_change);
						break;	
					case settimerminutes:
						change_timer_minute(direct_change);
						break;
					case settimerhours:
						change_timer_hour(direct_change);
						break;	
					default:
					;
				}
				prev_pair_bits = 0xff;
				equal_repeats = 0;
				encoder_byte = 0x00;
				flags_encoder &= 0b11111000;
			}
		}
		//start_timer1();
	}  // if (mode)
	PORT_TEST &= ~(1 << ONE_PIN_TEST2);
}

