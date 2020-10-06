/*
 * File:   ADC.c
 * Author: super
 *
 * Created on October 5th, 2020
 */
#include "ADC.h"
#include <xc.h>
#define    FCY    60000000UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>        // __delayXXX() functions macros defined here


unsigned int readADC(int channel) {
  unsigned int result;

  // Turn off the ADC before we start setting the registers
  ADCON1bits.ADON = 0;

  IFS0bits.ADIF = 0; // Clear the ADC interrupt flag
  IPC2bits.ADIP = 0; // Clear the interrupt priority
  IEC0bits.ADIE = 0; // Disable ADC interrupt

  // Start by setting each of the six 16-bit registers for desired operation
  // The constants used to set the registers are defined in adc12.h
  // See the dsPIC30F family manual, section 18, for more information on the ADC registers.

  // A/D Input Select Register (ADCHS)
    // CH0NA - Negative reference; can be either internal or analog channel 0.
    // CH0SA - Positive value to measure; a number between 0 and 7 (since we have 8 analog inputs)
  ADCHSbits.CH0NA = 0; // Use internal negative reference
  ADCHSbits.CH0SA = channel; // Use the given pin as the positive value

  // A/D Port Configuration Register (ADPCFG)
  // We don't change any Analog/Digital pin settings here, because
  // they should have been setup earlier using the pinXType aliases.

  // A/D Input Scan Selection Register (ADCSSL)
  // This function only grabs one value, so we'll turn it off
  ADCSSL = 0x00;

  // A/D Control Register 3 (ADCON3)
    // SAMC - Sample time delay (not used with manual sampling)
    // ADRC - Clock source select
    // ADCS - Conversion clock speed
  //ADCON3 = 0xe0ff & 0xff7f & 0xffc9;
  ADCON3bits.SAMC = 0b0000; // no sample delay
  ADCON3bits.ADRC = 0;      // internal RC clock
  ADCON3bits.ADCS = 15;

  // A/D Control Register 2 (ADCON2)
    // VCFG - Voltage reference configuration
    // SMPI - Samples between interrupts (up to 16; in the mean time, the values will
    //        be placed in the buffer
    // ALTS - Alternate input sample mode - switches between the mux inputs on each sample
    // BUFM - Buffer mode; 2 8-word buffers or one 16-bit one
    // CSCNA - Scan input selections?
    // BUFS - Buffer fill status (read-only)
  //ADCON2 = 0x0fff & 0xefc3 & 0xeffd & 0xeffe & 0xebff;
  ADCON2bits.VCFG = 0b000;   // set voltage references
  ADCON2bits.CSCNA = 0;      // do not scan inputs
  ADCON2bits.SMPI = 0b0000;  // interrupt at the completion of each conversion
  ADCON2bits.BUFM = 0;       // 16 bit one word buffer
  ADCON2bits.ALTS = 0;       // always use default pin assignments 

  // A/D Control Register 1 (ADCON1)
    // ADON - Module on/off
    // ADSIDL - Idle mode operation
    // FORM - Output format
    // SSRC - Conversion trigger selection
    //        When SSRC<2:0> = 000, the conversion trigger is under software control.
    //        Clearing the SAMP bit will cause the conversion trigger.
    // ASAM - Auto sampling; begin a new conversion as soon as one ends
    // SAMP - Enable sampling
    // DONE - Conversion complete (read-only)
  //  ADCON1 = 0xffff & 0xfcff & 0xff1f & 0xfffb & 0xfffd;
  ADCON1bits.ADSIDL = 1;   // discontinue operation in idle
  ADCON1bits.FORM = 0b00;  // the data output format is an integer
  ADCON1bits.SSRC = 0b000; // conversion trigger is under software control
  ADCON1bits.ASAM = 0;     // sampling begins when SAMP is set

  ADCON1bits.ADON = 1; // turn on the module 

  //Let the ADC turn on and stabilize at least 20us.
  __delay_us(300);

  // Begin sampling and wait
  ADCON1bits.SAMP = 1;
  __delay_us(3000);

  // End sampling and start the conversion
  ADCON1bits.SAMP = 0;

  // Wait for the conversion to finish
  while(ADCON1bits.DONE == 0);

  // Get the value from the buffer
  result = ADCBUF0;

  // Turn off the ADC
  //ADCON1bits.ADON = 0;

  return(result);
} 
    
    
  