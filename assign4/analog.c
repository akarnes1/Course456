// Quick Demo of light sensor

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "pinDefines.h"

// -------- Functions --------- //
static inline void initADC0(void) {
        ADMUX |= (1 << REFS0);            /* reference voltage on AVCC */
        ADCSRA |= (1 << ADPS2);             /* ADC clock prescaler /16 */
        ADCSRA |= (1 << ADEN);                           /* enable ADC */
}

int main(void) {

        // -------- Inits --------- //
        uint8_t ledValue;
        uint16_t adcValue;

        initADC0();
        DDRB |= 0b00000111;
        DDRD |= 0b10000000;

        initUSART();
        printString("Hello World!\r\n");

        // ------ Event loop ------ //
        while (1) {

                ADCSRA |= (1 << ADSC);         /* start ADC conversion */
                loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
                adcValue = ADC;                         /* read ADC in */
                /* Have 10 bits, want 3 (eight LEDs after all) */
                ledValue = (adcValue >> 7);
                /* Light up all LEDs up to ledValue */


                if(bit_is_set(UCSR0A, RXC0)) {
                        if(receiveByte() == 'd') {
                                PORTD ^= (1<<7);
                                _delay_ms(100);
                        }
                }
                if(PIND & (0b00100000)) {
                        printByte(ledValue);
                        printString("\n\r");
                }

                PORTB = ledValue;
                _delay_ms(50);
        }                                            /* End event loop */
        return 0;                      /* This line is never reached */
}
