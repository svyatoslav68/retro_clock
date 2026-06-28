/*
 * eeprom_values.c
 *
 * Created: 24.05.2026 14:42:54
 *  Author: Святослав
 */ 
#include <avr/eeprom.h>
#include "data_to_display.h"

uint8_t EEMEM digits[] = {IND_ZERO, IND_ONE, IND_TWO, IND_THREE, IND_FOUR, IND_FIVE, IND_SIX, IND_SEVEN, IND_EIGHT, IND_NINE, IND_DOT, IND_MINUS};
uint8_t EEMEM eeprom_alarm[LENGTH_ARRAY] = {0,0};
uint8_t EEMEM eeprom_timer[LENGTH_ARRAY] = {0,5};