#ifndef ENCODER_H
#define ENCODER_H

#define DDR_ENCODER DDRD
#define DDR_TEST DDRB
#define PORT_TEST PORTB
#define PORT_ENCODER PORTD
#define PIN_ENCODER PIND

#define ENCODER_CHANNEL_A PORTD4
#define ENCODER_CHANNEL_B PORTD5
#define ONE_PIN_TEST PORTB0

#define NUMBER_RIGHT_VALUE 2
#define NUMBER_FOR_END_ROTATION 20

void init_encoder();
void reading_encoder();

#endif // ENCODER_H
