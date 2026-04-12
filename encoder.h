#ifndef ENCODER_H
#define ENCODER_H

#define DDR_ENCODER DDRD
#define DDR_TEST DDRD
#define PORT_TEST PORTD
#define PORT_ENCODER PORTD
#define PIN_ENCODER PIND

#define ENCODER_CHANNEL_A PORTD0
#define ENCODER_CHANNEL_B PORTD1
#define ONE_PIN_TEST1 PORTD4
#define ONE_PIN_TEST2 PORTD5

#define NUMBER_RIGHT_VALUE 2
#define NUMBER_FOR_END_ROTATION 20

void init_encoder();
void reading_encoder();

#endif // ENCODER_H
