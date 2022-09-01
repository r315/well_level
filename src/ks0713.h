/*
 *  KS0713 library
 *  Hugo Reis 2013
 *
 * This software is free you can edit modify or redistribute it, under the 
 * terms of General Public License 
 * 
 * This software is distributed WITHOUT ANY WARRANTY
 */

#ifndef _KS0713
#define _KS0713
/*******************************************
 * Board - pro59801 
 * Controller - KS0713(S6B1713)
 * LCD - BP12864A
 * Mode - serial
 * 
 * 1 GND
 * 2 VCC
 * 3 NC
 * 4 /RSTB   - RST
 * 5 /CS1    - GND
 * 6 RS
 * 7 RW_WR   - GND
 * 8 E_RD    - GND
 * 9-14 DB0:5 
 * 15 DB6    <- SCL
 * 16 DB7    <- SDA
 * 17 led+
 * 18 led-
 *******************************************/

#define KS0713_CASET  		0x10  // MSB
#define KS0713_RRS	        0x20  // Regulator Resistor Select
#define KS0713_PC			0x28  // Power Control
#define KS0713_DRAM   		0x40  // Display ram
#define KS0713_RMODE  		0x81  // set reference mode
#define KS0713_PASET  		0xB0
#define KS0713_SEG 			0xA0  // Scan direction A0:SEG1->SEG132, A1: SEG132->SEG1
#define KS0713_BIAS			0xA2  // Selects LCD bias ratio of the voltage required for driving the LCD
#define KS0713_ALL_OFF 		0xA4
#define KS0713_ALL_ON 		0xA5
#define KS0713_REVERSE_OFF	0xA6
#define KS0713_REVERSE_ON	0xA7
#define KS0713_SSIM			0xAC // Set Static Indicator Mode
#define KS0713_DISP_OFF   	0xAE
#define KS0713_DISP_ON   	0xAF
#define KS0713_COM  		0xC0 // C0:COM1->COM64, C8: COM64->COM1
#define KS0713_RESET  		0xE2

#define KS0713_STATUS_BUSY	(1<<7)
#define KS0713_STATUS_ADC	(1<<6)
#define KS0713_STATUS_ON	(1<<5)
#define KS0713_STATUS_RSTB	(1<<4)

#define KS0713_PC_VF		(1<<0)
#define KS0713_PC_VR		(1<<1)
#define KS0713_PC_VC		(1<<2)

#define KS0713_SSIM_SM		(1<<0)
#define KS0713_SSIM_OFF		(0<<0)
#define KS0713_SSIM_ON1		(1<<0)
#define KS0713_SSIM_ON05	(2<<0)
#define KS0713_SSIM_ON		(3<<0)

#define KS0713_SEG_ADC		(1<<0)
#define KS0713_COM_SHL		(1<<3)

#define LCD_W 128
#define LCD_H 64

#define BUF_INC_ONE     1
#define BUF_INC_PAGE    LCD_W

void LCD_Init(void);

/**
 * @brief Set page and column address for write
 * 
 * @param x     Column address 255-0
 * @param y     Page address    15-0
 */
void LCD_setPos(uint8_t x, uint8_t y);

/**
 * @brief Write byte to current page and column address
 * 
 * @param d 
 */
void LCD_Data(uint8_t d);

/**
 * @param x : start column
 * @param y : start page
 * @param buf : data source
 * @param cols : number of columns to write
 * @param rows : number of rows to write
 * */
void LCD_WriteBuf(uint8_t x, uint8_t y, uint8_t *buf, uint8_t cols, uint8_t rows);


/**
 * @param x : start x coodinate
 * @param y : start y coordinate
 * @param buf : frame buffer data
 * @param w : width in pixels
 * @param h : height in pixels
 *
 * */
void LCD_PartialFrameBuffer(uint16_t x, uint16_t y, uint8_t *fbuf, uint16_t w, uint16_t h);
#endif
