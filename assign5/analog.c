// Quick Demo of light sensor

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int direction;
volatile int debounce;

ISR(INT1_vect){
        if(debounce == 0) {
                direction = direction - (2*direction) + 1;
                debounce = 1;
                PORTB = 0b11111111;
        }
}

int main(void) {

        // -------- Inits --------- //
        uint8_t ledValue = 0;
        debounce = 0;
        direction = 1;

        DDRD = 0<<PD3;    // Set PD2 as input (Using for interupt INT0)
        PORTD |= 1<<PD3;

        EIMSK |= 1<<INT1;
        EICRA |= 1<<ISC00;           // Enable External Interrupt 1
        sei();

        DDRB |= 0b11111111;

        // ------ Event loop ------ //
        while (1) {
                if(direction == 1) {
                        ledValue += 1;
                } else {
                        ledValue -= 1;
                }
                PORTB = ledValue;
                _delay_ms(1000);
                debounce = 0;
        }                                            /* End event loop */
        return 0;                      /* This line is never reached */
}
