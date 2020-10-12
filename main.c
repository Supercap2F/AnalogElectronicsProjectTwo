/*
 * File:   main.c
 * Author: super
 *
 * Created on September 28, 2020, 4:16 PM
 */
#pragma config FOSFPR = FRC_PLL8       // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF             // Watchdog Timer (Enabled)
// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)
// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)
// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

#include <xc.h>

#include "I2C\I2C.h"
#include "Graphics\SSD1306.h"
#include "Graphics\test.h"
#include "Graphics\GDGL.h"
#include "ADC\ADC.h"

#define FCY 60000000UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>        // __delayXXX() functions macros defined here
#include "Graphics\numberConverter.h"

/***********************************************
 * Definitions                                 *
 ***********************************************/
// NOTE: the pins that the interface to the LCD are defined in the LCD driver file 

int main(void) {
    unsigned int rint =0;
    unsigned int r1, r2, r3, r4, sel1, sel2;
    double rat = 0;
    double result;
    char charResult[20];
    /***********************************************
     * oscillator configuration                    *
     ***********************************************/
    
    
    /***********************************************
     * I/O pin configuration                       *
     ***********************************************/
    /***********************************************
     * I/O connections:                            *
     ***********************************************
     *                   PORTB                     
     *   RB<0:9> Photo resistors
     ***********************************************
     *                   PORTC                     
     * N/C
     ***********************************************
     *                   PORTD                     
     * N/C
     ***********************************************
     *                   PORTF
     * RF2 - SDA
     * RF3 - SCL
     ***********************************************/
    ADPCFG=0b0000000000000000; // Turn on the analog feature on all of PORTB
    TRISB =0b1111111111111111; // set all of port b to inputs
    LATB  =0b0000000000000000; // set all PORTB outputs low 
    
    //ANSELA=0b0000 0000 0000 0000; // turn off the analog features on all PORT pins
    TRISC =0b0000000000000000; //  
    LATC  =0b0000000000000000; // 
    
    TRISD =0b0000000000000000; // set all of PORTD to be 
    LATD  =0b0000000000000000;
    
    TRISF =0b0000000000000000;
    LATF  =0b0000000000000000;
   
    I2C_Setup();
    OLED_Setup();
    OLED_ClearDisplay();
    
    SetTextSize(1);
    OLED_UpdateDisplay(); 
    
    /*
    while(1) {
        rint = (unsigned int) readADC(0);
        intToStr(rint, charResult, 1);
        WriteString(2, 12, "int: ", ON, OFF);
        WriteString(32, 12, charResult, ON, OFF);
        
        
        result = rint / (4095.0);
        ftoa(result, charResult , 3);
        WriteString(2, 2, "flt: ", ON, OFF);
        WriteString(32, 2, charResult , ON, OFF);
    
        
        OLED_UpdateDisplay();  
        OLED_ClearDisplay();
        
        __delay_ms(100);
    }*/
    
    
    while(1) {
        // get values from the ADC
        r1 = (unsigned int) readADC(0); // get the value from the top resistor
        r2 = (unsigned int) readADC(1); // get the value from the right resistor
        r3 = (unsigned int) readADC(2); // get the value from the bottom resistor
        r4 = (unsigned int) readADC(3); // get the value from the left resistor 
        
        if(r1 > r3) {
            if(r2 > r4) {
                sel2 = r1;
                sel1 = r2;
                rat = 0;
            } else {
                sel2 = r4;
                sel1 = r1;
                rat = 270;
            }
            
        }else {
            if(r2 > r4) {
                sel2 = r2;
                sel1 = r3;
                rat = 90;
            } else {
                sel2 = r4;
                sel1 = r3;
                rat = 180;
            }
        }
        
        // calculate angle 
        result = (((double) sel1/sel2) * 45) + rat;
        
        
        ftoa(result, charResult , 3);
        WriteString(2, 2, "angle: ", ON, OFF);
        WriteString(50, 2, charResult , ON, OFF);
        PlotCircle(95, 2, 2, ON);
        //WriteString(93, 2, "deg", ON,OFF);
        
        
        OLED_UpdateDisplay();  
        OLED_ClearDisplay();
        __delay_ms(100);
    }
    
    
    
    
    
    
    
    
    
}

