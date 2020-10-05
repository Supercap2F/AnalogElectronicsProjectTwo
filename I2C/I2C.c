/*
 * File:   I2C.c
 * Author: super
 *
 * Created on September 30, 2020, 4:41 PM
 */
#include "I2C.h"
#include <xc.h>



// functions
void I2C_Setup(void) {
    I2CBRG = 272;         // baud rate
    I2CCONbits.I2CEN = 1; // turn on the I2C module and release the lines
}


void I2C_Start(void) {
    //I2CSTAT contains the status of the I2C module
    // while(!I2CSTATbits.P) bus is in an idle state
    // initiate a start condition
    I2CCONbits.SEN = 1;
    while(I2CCONbits.SEN); // wait for the send bit to finish
    // send device address
    
}

void I2C_Write(unsigned int m) {
    I2CTRN = m; // write data here
    // wait for and verify ack
    while(I2CSTATbits.TRSTAT); // wait until the buffer is empty
    
}


void I2C_End(void) {
    I2CCONbits.PEN = 1; // generate stop condition
    while(I2CCONbits.PEN); // wait for the stop condition to end
    
}