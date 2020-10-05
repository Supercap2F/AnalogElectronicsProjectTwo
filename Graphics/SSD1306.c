/************************************************
 * this file contains the source code for the   *
 * functions: OLED_SetAddr, OLED_WriteChar, and *
 * OLED_WriteString.                            *
 *                                                                                 *
 * Copyright (c) 2016, Supercap2F                                                  *
 * All rights reserved.                                                            *
 *                                                                                 *
 * Redistribution and use in source and binary forms, with or without              *     
 * modification, are permitted provided that the following conditions are met:     *
 *                                                                                 *
 *    * Redistributions of source code must retain the above copyright notice,     *
 *      this list of conditions and the following disclaimer.                      *
 *                                                                                 *
 *    * Redistributions in binary form must reproduce the above copyright notice,  *
 *      this list of conditions and the following disclaimer in the documentation  *
 *      and/or other materials provided with the distribution.                     *
 *                                                                                 *
 *    * Neither the name of 16Bit-PIC-GDGL nor the names of its                    *
 *      contributors may be used to endorse or promote products derived from       *
 *      this software without specific prior written permission.                   *          
 *                                                                                 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            *
 *                                                                                 *
 * Project repo here:                                                              *
 *      https://github.com/Supercap2F/16Bit-PIC-GDGL                               *
 ***********************************************************************************/
/************************************************
 * Included files                               *
 ************************************************/
#include "SSD1306.h"
#include "../I2C/I2C.h"
#include <xc.h>

/************************************************
 * Global variables                             *
 ************************************************/
unsigned int current_page=0;
unsigned int current_column=0;
unsigned char ScrnBuff[((Scrn_H+1)/8)][((Scrn_W+1))];
/************************************************
 * OLED_PlotPoint function                      *
 * this function updates the screen buffer with 
 * new data 
 ************************************************/
int PlotPoint(int x, int y, unsigned char state) {
    if ((x < 0) || (x >= (Scrn_W+1)) || (y < 0) || (y >= (Scrn_H+1))) // if sent a pixel not on the screen
        return(OLED_OUTOFRANGE);                                      // return error 
    
    switch(state) {
        case ON: 
            ScrnBuff[(y/8)][x]|=(1<<(y&7));
            break;
        case OFF:
            ScrnBuff[(y/8)][x]&=~(1<<(y&7));
            break;
        case IVRT:
            ScrnBuff[(y/8)][x]^=(1<<(y&7));
            break;       
    }
    
    // uncomment these lines if the update display function is not in use
    //OLED_SetAddr((y/8),x);
    // OLED_WriteDATA(ScrnBuff[(y/8)][x]);
   
    return(OLED_SUCCESS);
}

/************************************************
 * OLED_UpdateDisplay function                  *
 * This function writes the contents of the 
 * screen buffer to the display
 ************************************************/
void OLED_UpdateDisplay(void) {
    int x,y;
    // move first pixel of the display
    OLED_SetAddr(0,0);
    
    // this function directly uses the I2C functions to speed up the
    // data transfer process.
    I2C_Start();
    I2C_Write(slaveAddrW); // send the address
    
    I2C_Write(0b01000000); // tell the display that its a continued data byte transmission
    
    // write the contents of the screen buffer to the display 
    for(y=0;y<4;y++) {
        for(x=0;x<128;x++) {
            I2C_Write(ScrnBuff[y][x]);
        }
    }
    
    I2C_End(); // end the transmission
}

void updateScrnBuff(unsigned char cmd, int x, int y) {
    ScrnBuff[y][x] = cmd;
}

/************************************************
 * OLED_SetAddr function                        *
 ************************************************/
int OLED_SetAddr(int page, int column) {
    if(column>127||column<0||page>3||page<0) // if the function is sent an invalid column/page address
        return(OLED_OUTOFRANGE);            // return error
    
    current_page=page;      // update current address variables
    current_column=column;
    
    OLED_WriteCMD(0x21);    // write new column address
    OLED_WriteCMD(column);  //
    OLED_WriteCMD(127);     // the end column address will still be 127 though 
    
    OLED_WriteCMD(0x22);    // write the new page address
    OLED_WriteCMD(page);    //
    OLED_WriteCMD(3);       // the end page address will still be 7 though
    
    return(OLED_SUCCESS);   // return success code 
}


/***************************************************
 * function OLED_Write... - These functions write  *
 * commands/data to the OLED display               *
 ***************************************************/
void OLED_WriteCMD(unsigned int cmd) {
    I2C_Start();
    I2C_Write(slaveAddrW);
    
    I2C_Write(0b10000000); // tell the display that its a command
    I2C_Write(cmd);        // write the command
    
    I2C_End();
}

void OLED_WriteDATA(unsigned int cmd) {
    I2C_Start();
    I2C_Write(slaveAddrW);
    
    I2C_Write(0b01000000); // tell the display that its a data byte
    I2C_Write(cmd);        // write the command
    
    I2C_End();
}


/************************************************
 * OLED_Setup function                          *
 * This function writes all of the setup commands
 * to the screen so that it will function properly.
 * The commands here must be changed for different 
 * types of displays (including different sizes).
 * See the data sheet for documentation on how to 
 * set these up
 ************************************************/
void OLED_Setup(void) {
    OLED_WriteCMD(0xAE); //display off
	OLED_WriteCMD(0x20); //Set Memory Addressing Mode   
	OLED_WriteCMD(0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCMD(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCMD(0xC8); //Set COM Output Scan Direction
    
	OLED_WriteCMD(0x00); //---set low column address
	OLED_WriteCMD(0x10); //---set high column address
    
	OLED_WriteCMD(0x40); //--set start line address
	OLED_WriteCMD(0x81); //--set contrast control register
	OLED_WriteCMD(0x8F);
	OLED_WriteCMD(0xA1); //--set segment re-map 0 to 127
	OLED_WriteCMD(0xA6); //--set normal display
	OLED_WriteCMD(0xA8); //--set multiplex ratio(1 to 64)
	OLED_WriteCMD(0x1F); //
	OLED_WriteCMD(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WriteCMD(0xD3); //-set display offset
	OLED_WriteCMD(0x00); //-not offset
	OLED_WriteCMD(0xD5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCMD(0xF0); //--set divide ratio
	OLED_WriteCMD(0xD9); //--set pre-charge period
	OLED_WriteCMD(0x22); //
	OLED_WriteCMD(0xDA); //--set com pins hardware configuration
	OLED_WriteCMD(0x02);
	OLED_WriteCMD(0xDB); //--set vcomh
	OLED_WriteCMD(0x40); //0x20,0.77xVcc
	OLED_WriteCMD(0x8D); //--set DC-DC enable
	OLED_WriteCMD(0x14); //
	OLED_WriteCMD(0xAF); //--turn on SSD1306 panel
}

/************************************************
 * OLED_ClearDisplay function                   *
 ************************************************/
void OLED_ClearDisplay(void) {
    int x,y;
    
    OLED_SetAddr(0,0);
    for(y=0;y<4;y++) {
        for(x=0;x<128;x++) {
            OLED_WriteDATA(0x00);
            ScrnBuff[y][x]=0x00;
        }
    }
}

