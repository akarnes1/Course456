/* Reads LM75 Thermometer and Prints Value over Serial */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#include "pinDefines.h"
#include "USART.h"
#include "i2c.h"

// -------- Defines -------- //
#define ADX_POWER_CONTROL           0x2D
#define ADX_ADDRESS_W               0xA6
#define ADX_ADDRESS_R               0xA7
#define ADX_FULL_RESOLUTION         0x31
#define ADX_X_DATA_LOW              0x32
#define ADX_X_DATA_HIGH             0x33
#define ADX_Y_DATA_LOW              0x34
#define ADX_Y_DATA_HIGH             0x35
#define ADX_Z_DATA_LOW              0x36
#define ADX_Z_DATA_HIGH             0x37

volatile uint8_t xHigh, xLow, yHigh, yLow, zHigh, zLow;
// -------- Functions --------- //

void adxlInit(){
        /* To set register, address LM75 in write mode */
        i2cStart();
        i2cSend(ADX_ADDRESS_W);
        i2cSend(ADX_POWER_CONTROL);
        i2cSend(1<<3);
        i2cSend(ADX_FULL_RESOLUTION);
        i2cSend(1<<3);
        i2cStop();
}

void adxlReadX(){
        /* Setup and send address, with read bit */
        i2cStart();
        i2cSend(ADX_ADDRESS_W);
        i2cSend(ADX_X_DATA_LOW);

        i2cStart();
        i2cSend(ADX_ADDRESS_R);
        /* Now //'receive two bytes of temperature */
        //xLow = i2cReadAck();
        xHigh = i2cReadNoAck();
        i2cStop();
        printString("\n\rX: ");
        printByte(xHigh);
        //printByte(xLow);
}

void adxlReadY(){
        /* Setup and send address, with read bit */
        i2cStart();
        i2cSend(ADX_ADDRESS_W);
        i2cSend(ADX_Y_DATA_LOW);

        i2cStart();
        i2cSend(ADX_ADDRESS_R);
        /* Now //'receive two bytes of temperature */
        //yLow = i2cReadAck();
        yHigh = i2cReadNoAck();
        i2cStop();
        printString("\n\rY: ");
        printByte(yHigh);
        //printByte(yLow);
}

void adxlReadZ(){
        /* Setup and send address, with read bit */
        i2cStart();
        i2cSend(ADX_ADDRESS_W);
        i2cSend(ADX_Z_DATA_LOW);

        i2cStart();
        i2cSend(ADX_ADDRESS_R);


        /* Now //'receive two bytes of temperature */

        //zLow = i2cReadAck();
        zHigh = i2cReadNoAck();
        i2cStop();
        printString("\n\rZ: ");
        printByte(zHigh);
        //printByte(zLow);
}

int main(void) {
// -------- Inits --------- //
        //clock_prescale_set(clock_div_1);  /* 8MHz */
        initUSART();
        printString("\n\r====  i2c Accelerometer  ====\r\n");
        initI2C();
        _delay_ms(1000);
        printString("i2c init\n\r");
        adxlInit();
        printString("adxl Init\n\r");
        _delay_ms(500);

// ------ Event loop ------ //
        while (1) {
                // Print it out nicely over serial for now...
                adxlReadX();
                adxlReadY();
                adxlReadZ();
                printString("\n\r");
                printString("\n\r");

                /* Once per second */
                _delay_ms(100);

        }                                          /* End event loop */
        return 0;                    /* This line is never reached */
}
