#ifndef ENCODER_H
#define ENCODER_H

#define DDR_ENCODER DDRD
#define PORT_ENCODER PORTD
#define PIN_ENCODER PIND

#define ENCODER_CHANNEL_A PORTD1
#define ENCODER_CHANNEL_B PORTD0

#define NUMBER_RIGHT_VALUE 4
#define NUMBER_FOR_END_ROTATION 20

void init_encoder();
void reading_encoder();
void enable_encoder();
void disable_encoder();

#endif // ENCODER_H
