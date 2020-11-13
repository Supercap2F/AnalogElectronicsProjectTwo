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
#pragma config WDT = WDT_OFF             // Watchdog Timer (disabled)
// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF          // PBOR Enable (Enabled)
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
    unsigned int sel1, sel2;
    double rat = 0;
    double result;
    char charResult[20];
    int x;
    
    int highest = 0;
    int low,high;
    unsigned int values[11];
    
    double elevation = 0;
    double eleangle = 0;
    
    int extraspace = 0;
    
    int plusFlag = 0;
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
    
    
    while(1) {
        // get values from the ADC
        for(x =0; x < 11; x ++ ){
            values[x] = (unsigned int) readADC(x);
        }
        //r1 = (unsigned int) readADC(0); // get the value from the top resistor
        highest = 0;
        for( x = 0; x<8; x++) {
            if(values[highest] < values[x]) {
                highest = x;
            }
            
        }
 
        if(highest == 0) {
            low = 7;
        } else {
            low = highest - 1;
        }
        if(highest == 7) {
            high = 0;
        } else {
            high = highest + 1;
        }
        
        if(values[low] < values[high]) {
            sel2 = values[high];
            sel1 = values[highest];
            rat = (highest ) * 45;
            
        } else {
            sel1 = values[low];
            sel2 = values[highest];
            rat = (low ) * 45;
        }
        // calculate angle 
        result = (((double) sel1/sel2) * 22.5) + rat;
        
        // calculate elevation
        elevation =(double)(values[8]+values[9]+values[10])/3;
        if(elevation > 2600) {
            eleangle = 0;
            plusFlag = 0;
        }
        else if(elevation > 2000) {
            elevation -= 2000;
            eleangle = ((600.0 - elevation)/600.0) * 8.0;
            plusFlag = 0;
        } else {
            eleangle = 8;
            plusFlag = 1;
        }
        
        
        // display the angle 
        ftoa(result, charResult , 3);    
        if(result > 99.999) { // calculate the shift if the number is less than three decimals long
            extraspace = 0;
        } else {
            extraspace = 11;
        }
        SetTextSize(2);
        WriteString(24 + extraspace, 0, charResult , ON, OFF);
        PlotFilledRectangle(103,0, 4, 4, ON);
        
        // display the elevation
        ftoa(eleangle, charResult , 2);  
        if(eleangle > 99.999) { // calculate the shift if the number is less than three decimals long
            extraspace = 0;
        } else if(eleangle > 9.999) {
            extraspace = 11;
        } else {
            extraspace = 22;
        }
        SetTextSize(2);
        WriteString(24 + extraspace, 18, charResult , ON, OFF);
        PlotFilledRectangle(103 - 11 ,18, 2, 4, ON);
        PlotFilledRectangle(106 - 11 ,18, 2, 4, ON);
        if(plusFlag) {
            WriteString(24 + 11, 18, "+" , ON, OFF);
        }
        
        
        // update the display
        OLED_UpdateDisplay();  
        OLED_ClearDisplay();
        __delay_ms(5);
    }
}

