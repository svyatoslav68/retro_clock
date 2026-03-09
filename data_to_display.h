#ifndef DATA_TO_DISPLAY_H
#define DATA_TO_DISPLAY_H

#define NUMBER_DIGIT 3

#define PINBOARD
/* Определение констант для отображение символов на семисегментном индикаторе */
#ifdef PINBOARD
#define	IND_ZERO	0b11101011	/* 0xEB Отображение "0"*/
#define	IND_ONE		0b00101000	/* 0x28 Отображение "1"*/
#define	IND_TWO		0b10110011	/* 0xB3 Отображение "2"*/
#define	IND_THREE	0b10111010	/* 0xBA Отображение "3"*/
#define	IND_FOUR	0b01111000	/* 0x78 Отображение "4"*/
#define	IND_FIVE	0b11011010	/* 0xCA Отображение "5"*/
#define	IND_SIX		0b11011011	/* 0xCB Отображение "6"*/
#define	IND_SEVEN	0b10101000	/* 0xA8 Отображение "7"*/
#define	IND_EIGHT	0b11111011	/* 0xFB Отображение "8"*/
#define	IND_NINE	0b11111010	/* 0xFA Отображение "9"*/
#define	IND_DOT		0b00000100	/* 0x80 Отображение "."*/
#define	IND_MINUS	0b00010000	/* 0x40 Отображение "-"*/
#define IND_CEL		0b11110000  /* 0x63 Отображение знака градус */
#define IND_E		0b11010011
#define IND_R		0b00010001
#define IND_S		0b11011010  /* 0x6B Отображение "S" */
#define IND_H       0b01111001  /* 0x76 Отображение "H" */
#define IND_T       0b01010011  /* 0x78 Отображение "t" */
#define IND_CLEAR	0b00000000
#else
#define	IND_ZERO	0b00111111	/* 0x3F Отображение "0"*/
#define	IND_ONE		0b00000110	/* 0x06 Отображение "1"*/
#define	IND_TWO		0b01011011	/* 0x5B Отображение "2"*/
#define	IND_THREE	0b01001111	/* 0x4F Отображение "3"*/
#define	IND_FOUR	0b01100110	/* 0x66 Отображение "4"*/
#define	IND_FIVE	0b01101101	/* 0x6D Отображение "5"*/
#define	IND_SIX		0b01111101	/* 0x7D Отображение "6"*/
#define	IND_SEVEN	0b00000111	/* 0x07 Отображение "7"*/
#define	IND_EIGHT	0b01111111	/* 0xEF Отображение "8"*/
#define	IND_NINE	0b01101111	/* 0x6F Отображение "9"*/
#define	IND_DOT		0b10000000	/* 0x80 Отображение "."*/
#define	IND_MINUS	0b01000000	/* 0x40 Отображение "-"*/
#define IND_CEL		0b01100011  /* 0x63 Отображение знака градус */
#define IND_E		0b01111001
#define IND_R		0b01010000
#define IND_S		0b01101101  /* 0x6B Отображение "S" */
#define IND_H       0b01110110  /* 0x76 Отображение "H" */
#define IND_T       0b01111000  /* 0x78 Отображение "t" */
#define IND_CLEAR	0b00000000
#define ON_DIGIT 	0b10000000
#endif

//extern volatile uint8_t data_for_display[NUMBER_DIGIT];
void display_array(void);
void init_display(void);
void next_flash_digit(void);
void flash_digiting(void);

/* Назначаем порты, отвественные за вывод на дисплей */
#define DIRECT_DISPLAY DDRA
#define PORT_DISPLAY PORTA
#define DIRECT_DIGITS DDRB
#define PORT_DIGITS PORTB

#define PORT_DIGIT_0 5
#define PORT_DIGIT_1 6
#define PORT_DIGIT_2 7


#endif // DATA_TO_DISPLAY_H
