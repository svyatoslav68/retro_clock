#ifndef MAIN_H
#define MAIN_H

/* Определение режима отладки */
//#define DEBUG_INT0
//#define F_CPU 7372800
/* Именование портов ввода/вывода */
//#define PORT_ENCODER PORTD
#define PORT_LEDS_PINBOARD PORTD

//#define DIRECT_ENCODER DDRD
#define DIRECT_LEDS_PINBOARD DDRD




#define PORT_LEDS_BOARD PORTA
#define DIRECT_LEDS_BOARD DDRA
#define PORT_LED_BOARD0 PORTA4
#define PORT_LED_BOARD1 PORTA5
#define PORT_LED_BOARD2 PORTA6
#define PORT_LED_BOARD3 PORTA7


/*
#define PIN_ENCODER_INPUT_1 PORTD2
#define PIN_ENCODER_INPUT_2 PORTD6
*/

//#define PORT_TEST PORTD4

/* Определение портов, к которым подключены тестовые светодиоды */
#ifdef PINBOARD
#define PORT_LEDS PORT_LEDS_PINBOARD
#define DIRECT_LEDS DIRECT_LEDS_PINBOARD
#define PORT_SWITCH_LED PORTD5
#define PORT_BLANK_LED PORT_TEST
#else
#define PORT_LEDS PORT_LEDS_BOARD
#define DIRECT_LEDS DIRECT_LEDS_BOARD
#define PORT_SWITCH_LED PORT_LED_BOARD1
#define PORT_BLANK_LED PORT_LED_BOARD0
#endif


/* Запрет прерываний */
#define CLI_M16 cli()
/* Разрешение прерываний */
#define SEI_M16 sei()
#endif // MAIN_H
