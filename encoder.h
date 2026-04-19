#ifndef ENCODER_H
#define ENCODER_H

#define DDR_ENCODER DDRD
#define DDR_TEST DDRD
#define PORT_TEST PORTD
#define PORT_ENCODER PORTD
#define PIN_ENCODER PIND

#define ENCODER_CHANNEL_A PORTD0
#define ENCODER_CHANNEL_B PORTD1
/* Пины для отладки */
#define ONE_PIN_TEST1 PORTD4
#define ONE_PIN_TEST2 PORTD5
#define ONE_PIN_TEST3 PORTD6
#define ONE_PIN_TEST4 PORTD7

#define NUMBER_RIGHT_VALUE 4
#define NUMBER_FOR_END_ROTATION 20

void init_encoder();
void reading_encoder();

#endif // ENCODER_H
