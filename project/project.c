/* Quick interactive demo running servo with Timer 1 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

#define PULSE_MIN         1380         /* experiment with these values */
#define PULSE_MAX         1620              /* to match your own servo */
#define PULSE_MID         1550

int volatile left = 0;
int volatile right = 0;

//Setup the Timer1 for use with controlling the servos
static inline void initTimer1Servo(void) {
        /* Set up Timer1 (16bit) to give a pulse every 20ms */
        /* Use Fast PWM mode, counter max in ICR1 */
        TCCR1A |= (1 << WGM11);
        TCCR1B |= (1 << WGM12) | (1 << WGM13);
        TCCR1B |= (1 << CS10); /* /1 prescaling -- counting in microseconds */
        ICR1 = 20000;                            /* TOP value = 20ms */
        TCCR1A |= (1 << COM1A1);      /* Direct output on PB1 / OC1A */
        DDRB |= (1 << PB1);                    /* set pin for output */
}

//Move the servos to verify that they work. (No longer works cause motors might be dead)
static inline void showOff(void) {
        OCR1A = PULSE_MID;
        _delay_ms(1500);
        OCR1A = PULSE_MIN;
        _delay_ms(1500);
        OCR1A = PULSE_MAX;
        _delay_ms(1500);
        OCR1A = PULSE_MID;
        _delay_ms(1500);
}

//Take the pulse length and move the servo there
static inline void servoMoveA(uint16_t pulseLength){
        OCR1A = pulseLength;
        DDRB |= (1<<PB1); /* re-enable output pin */
        while (TCNT1 < 3000) {;} /* delay until pulse part of cycle done */
        DDRB &= ~(1<<PB1);
}

//Take the pulse length and move the other servo
static inline void servoMoveB(uint16_t pulseLength){
        OCR1B = pulseLength;
        DDRB |= (1<<PB2); /* re-enable output pin */
        while (TCNT1 < 3000) {;} /* delay until pulse part of cycle done */
        DDRB &= ~(1<<PB2);
}

//read the value of the IR Recievers and update flag values.
static inline void poll(){
        if(bit_is_clear(PIND,PD1)) {
                left = 1;
        } else{
                left = 0;
        }
        if(bit_is_clear(PIND,PD2)) {
                right = 1;
        } else{
                right = 0;
        }
}


int main(void) {

// -------- Inits --------- //
        //Set IR Reciever pins to input
        DDRD |= 0b00001100;
        OCR1A = PULSE_MID;         /* set it to middle position initially */
        initTimer1Servo();
        initUSART();
        //showOff();

// ------ Event loop ------ //
        while (1) {
                poll();
                //Based on the poll results move the motors and print their state to the console
                if(left == 1) {
                        servoMoveA(1720);
                        printString("Left");
                }else{
                        servoMoveA(1550);
                        printString("Stop Left");
                }
                printString("\n\r");
                _delay_ms(50);
                if (right == 1) {
                        servoMoveB(1720);
                        printString("Right");
                }else{
                        servoMoveB(1550);
                        printString("Stop Right");
                }
                printString("\n\r");
                _delay_ms(1000);
        }                                          /* End event loop */
        return 0;                    /* This line is never reached */
}
