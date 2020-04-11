/*
 *  KS0713 library
 *  Hugo Reis 2013
 *
 * This software is free you can edit modify or redistribute it, under the 
 * terms of General Public License 
 * 
 * This software is distributed WITHOUT ANY WARRANTY
 */

#include <stdarg.h>
#include "board.h"
#include "ks0713.h"


void shiftout(uint8_t bits)
{
#if !defined(LCD_BIT_BANG)
	SPI(bits);
#else
    char i;
/*	LCDCLK0 if ((bits&128)) LCDSDI1 else LCDSDI0  LCDCLK1
	LCDCLK0 if ((bits&64 )) LCDSDI1 else LCDSDI0  LCDCLK1 	
  	LCDCLK0 if ((bits&32 )) LCDSDI1 else LCDSDI0  LCDCLK1 
  	LCDCLK0 if ((bits&16 )) LCDSDI1 else LCDSDI0  LCDCLK1 
  	LCDCLK0 if ((bits&8  )) LCDSDI1 else LCDSDI0  LCDCLK1 	
  	LCDCLK0 if ((bits&4  )) LCDSDI1 else LCDSDI0  LCDCLK1 
  	LCDCLK0 if ((bits&2  )) LCDSDI1 else LCDSDI0  LCDCLK1 
  	LCDCLK0 if ((bits&1  )) LCDSDI1 else LCDSDI0  LCDCLK1	*/
    for(i=0 ; i<8; i++, bits<<=1)
    {
        LCDCLK0;
        if(bits & 0x80)
        	LCDSDI1;
        else
        	LCDSDI0;
        LCDCLK1;
    }
#endif
}
//-----------------------------------------------
// 
//-----------------------------------------------
void lcdcmd(uint8_t c)
{
	LCDRS0;
	shiftout(c);
}
//-----------------------------------------------
// 
//-----------------------------------------------
void LCD_Data(uint8_t d)
{
	LCDRS1;
	shiftout(d);
}
//-----------------------------------------------
//
//-----------------------------------------------
void LCD_setPos(uint8_t x, uint8_t y)
{
	lcdcmd(0x10 | (x>>4));
	lcdcmd(x&0x0f);
	lcdcmd(0xB0 + y);
}
//-----------------------------------------------
//
//-----------------------------------------------
void LCD_Scroll(uint8_t line)
{
	line &= 0x3F;
	lcdcmd(KS0713_DRAM | line);
}
//-----------------------------------------------
//
//-----------------------------------------------
void LCD_Init(void)
{
	uint8_t w,h;

	LCDCLK0; 
	LCDRST0;
	DelayMs(250); 
	DelayMs(250);
	DelayMs(250);
	DelayMs(250);
	LCDRST1;
	
	LCDCS0;
	lcdcmd(KS0713_RESET);
	// SEG1->SEG132
	lcdcmd(KS0713_SEG);
   	// COM64->COM1
	lcdcmd(KS0713_COM | KS0713_COM_SHL);
	// select bias
	lcdcmd(KS0713_BIAS | 1);
	
	// Voltage converter on
	lcdcmd(KS0713_PC | KS0713_PC_VC);
	DelayMs(2);
	// Voltage regulator on
	lcdcmd(KS0713_PC | KS0713_PC_VC | KS0713_PC_VR);
	DelayMs(2);
	// voltage follower on
	lcdcmd(KS0713_PC | KS0713_PC_VC | KS0713_PC_VR | KS0713_PC_VF);
	
  	// regulator resistor
	lcdcmd(KS0713_RRS | 3);
  	// set reference voltage
	lcdcmd(KS0713_RMODE);
	// 0-63 reference voltage value (contrast)
	lcdcmd(38);
	
	lcdcmd(KS0713_DISP_ON);

	lcdcmd(KS0713_ALL_ON);

	for(h=0;h<8;h++){
		LCD_setPos(0,h);
		for(w=0;w<LCD_W;w++){
			LCD_Data(0x0);
		}
	}
	
	lcdcmd(KS0713_ALL_OFF);
	LCD_setPos(0,0);
}

/**
 * @param x : start column
 * @param y : start page
 * @param buf : data source
 * @param cols : number of columns to write
 * @param rows : number of rows to write
 * */
void LCD_WriteBuf(uint8_t x, uint8_t y, uint8_t *buf, uint8_t cols, uint8_t rows){
	if( (x + cols) > LCD_W || (y + rows) > (LCD_H / 8))
		return;

	for(uint8_t r = 0; r < rows; r++){
		LCD_setPos(x, y + r);
		for(uint8_t c = 0; c < cols; c++){
			LCD_Data(*buf++);
		}
	}
}

/**
 * @param x : start x coodinate
 * @param y : start y coordinate
 * @param buf : frame buffer data
 * @param w : width in pixels
 * @param h : height in pixels
 *
 * */
void LCD_PartialFrameBuffer(uint16_t x, uint16_t y, uint8_t *fbuf, uint16_t w, uint16_t h){
uint16_t page = y >> 3;

	if( (x + w) > LCD_W || (y + h) > (LCD_H))
		return;

	fbuf = fbuf + (page * LCD_W);

	for(; page <= (y + h - 1) >> 3; page++, fbuf += LCD_W){
		LCD_setPos(x, page);
		for(uint16_t col = x; col < x + w; col++){
			LCD_Data(*(fbuf + col));			
		}
	}
}
