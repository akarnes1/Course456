/* Blinker Demo */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

int main(void) {

        // -------- Inits --------- //
        DDRD |= 0b11111111;      /* Data Direction Register B:
                                    writing a one to the bit
                                    enables output. */
        DDRB = 0b00000000;      //Enable all data direction pins for C as input
        PORTB |= 0b00000001;

        int values = 0b10001000;
        int leds = 0b00000001;
        int loop = 0;
        int execute = 0;
        PORTD = leds;
        // ------ Event loop ------ //
        while (1) {
                if(PINB & (0b00000001)) {
                        execute = 1;
                        loop += 1;
                        _delay_ms(100);
                }
                if(execute > 0) {
                        switch (loop) {
                        case 1:
                                leds = (leds << 3);
                                break;
                        case 2:
                                leds = leds & values;
                                break;
                        case 3:
                                leds = leds | values;
                                break;
                        case 4:
                                leds = ~leds;
                                break;
                        case 5:
                                leds = leds ^ values;
                                loop = 0;
                                break;
                        }
                        PORTD = leds;
                        execute = 0;
                        _delay_ms(85);
                }
        }                                            /* End event loop */
        return 0;                      /* This line is never reached */
}
