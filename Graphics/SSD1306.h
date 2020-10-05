/************************************************
 * File:   OLED.h                               *
 * Author: Supercap2F                           *
 *                                              *
 * Created on June 8, 2015, 4:53 PM             *
 ************************************************/
#ifndef INC_OLED_H // inclusion guard 
#define	INC_OLED_H

/************************************************
 * definitions                                  *
 ************************************************/
#define OLED_SUCCESS    1
#define OLED_OUTOFRANGE 2 // out of range error 
#define OLED_PRNTCUTOFF 3 // if the print cant fit on the display 
#define OLED_DAT 1
#define OLED_CMD 0
#define ON 1
#define OFF 0
#define IVRT 2
#define Scrn_W 127
#define Scrn_H 31

#define slaveAddrW 0b01111000
#define slaveAddrR 0b01111001
/************************************************
 * NOTE: the user will need to change the       *
 * following definition values upon application *
 * requirements                                 *
 ************************************************ /
#define CS   LATAbits.LATA0 // 
#define RES  LATAbits.LATA1 //  
#define D_C  LATAbits.LATA2 //
#define RW   LATAbits.LATA3 //
#define E    LATAbits.LATA4 //
#define TRAN LATBbits.LATB7 // */

/************************************************
 * function prototypes                          *
 ************************************************/


void OLED_Setup(void);

int OLED_SetAddr(int page, int column);

void OLED_WriteCMD(unsigned int cmd);
void OLED_WriteDATA(unsigned int cmd);

int PlotPoint(int x, int y, unsigned char state);
void OLED_ClearDisplay(void);

void OLED_UpdateDisplay(void); // updates the screen buffer to the display

void updateScrnBuff(unsigned char cmd, int x, int y);

#endif	/* INC_OLED_H */

