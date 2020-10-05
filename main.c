/*
 * File:   main.c
 * Author: super
 *
 * Created on September 28, 2020, 4:16 PM
 */
#pragma config FOSFPR = FRC       // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_ON             // Watchdog Timer (Enabled)
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
//#include "Fonts\CherryCremeSodaFont.h"
/***********************************************
 * Definitions                                 *
 ***********************************************/
// NOTE: the pins that the interface to the LCD are defined in the LCD driver file 

int main(void) {
    int x,y;
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
    for(y=0;y<4;y++) {
        for(x=0;x<128;x++) {
            updateScrnBuff(test[x*y],x,y);
        }
    }
    OLED_UpdateDisplay();
    
    /*
    OLED_SetAddr(0,0);
    for(x=0;x<512;x++) {
            OLED_WriteDATA(dick[x]);
    }
    OLED_SetAddr(0,0);       
    */  
            
    while(1);
}
