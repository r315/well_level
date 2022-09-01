/*
 * font.c
 *
 *  Created on: 30/03/2020
 *      Author: hmr
 */

#include <stdint.h>
#include "text.h"
#include "board.h"

#define HIT_FONT_W 10
#define HIT_FONT_H 2
#define HITACHI_FONT_LINE_BYTES HIT_FONT_W * HIT_FONT_H

const uint8_t HITACHI_FONT[] = {

    0xFC, 0xFC, 0x03, 0x03, 0xC3, 0xC3, 0x33, 0x33, 0xFC, 0xFC, 0x0F, 0x0F, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30, 0x0F, 0x0F, // 48
    0x00, 0x00, 0x0C, 0x0C, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x3F, 0x3F, 0x30, 0x30, 0x00, 0x00, // 49
    0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x3C, 0x3C, 0x30, 0x30, 0x3C, 0x3C, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30, // 50
    0x03, 0x03, 0x03, 0x03, 0x33, 0x33, 0xCF, 0xCF, 0x03, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0F, 0x0F, // 51
    0xC0, 0xC0, 0x30, 0x30, 0x0C, 0x0C, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x3F, 0x03, 0x03, // 52
    0x3F, 0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xC3, 0xC3, 0x0C, 0x0C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0F, 0x0F, // 53
    0xF0, 0xF0, 0xCC, 0xCC, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x0F, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0F, 0x0F, // 54
    0x0F, 0x0F, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, // 55
    0x3C, 0x3C, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x3C, 0x3C, 0x0F, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0F, 0x0F, // 56
    0x3C, 0x3C, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFC, 0xFC, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x0C, 0x0C, 0x03, 0x03  // 57
};


#define FONT_W 5
#define FONT_SPACING 1

const uint8_t default_font[]={
     0x00, 0x00, 0x00, 0x00, 0x00 ,   // sp   0x20
     0x00, 0x00, 0x2f, 0x00, 0x00 ,   // !
     0x00, 0x07, 0x00, 0x07, 0x00 ,   // "
     0x14, 0x7f, 0x14, 0x7f, 0x14 ,   // #
     0x24, 0x2a, 0x7f, 0x2a, 0x12 ,   // $
     0xc4, 0xc8, 0x10, 0x26, 0x46 ,   // %
     0x36, 0x49, 0x55, 0x22, 0x50 ,   // &
     0x00, 0x05, 0x03, 0x00, 0x00 ,   // '
     0x00, 0x1c, 0x22, 0x41, 0x00 ,   // (
     0x00, 0x41, 0x22, 0x1c, 0x00 ,   // )
     0x14, 0x08, 0x3E, 0x08, 0x14 ,   // *
     0x08, 0x08, 0x3E, 0x08, 0x08 ,   // +
     0x00, 0x00, 0x50, 0x30, 0x00 ,   // ,
     0x08, 0x08, 0x08, 0x08, 0x08 ,   // -
     0x00, 0x60, 0x60, 0x00, 0x00 ,   // .
     0x20, 0x10, 0x08, 0x04, 0x02 ,   // /
     0x3E, 0x51, 0x49, 0x45, 0x3E ,   // 0  0x30
     0x00, 0x42, 0x7F, 0x40, 0x00 ,   // 1
     0x42, 0x61, 0x51, 0x49, 0x46 ,   // 2
     0x21, 0x41, 0x45, 0x4B, 0x31 ,   // 3
     0x18, 0x14, 0x12, 0x7F, 0x10 ,   // 4
     0x27, 0x45, 0x45, 0x45, 0x39 ,   // 5
     0x3C, 0x4A, 0x49, 0x49, 0x30 ,   // 6
     0x01, 0x71, 0x09, 0x05, 0x03 ,   // 7
     0x36, 0x49, 0x49, 0x49, 0x36 ,   // 8
     0x06, 0x49, 0x49, 0x29, 0x1E ,   // 9
     0x00, 0x36, 0x36, 0x00, 0x00 ,   // :
     0x00, 0x56, 0x36, 0x00, 0x00 ,   // ;
     0x08, 0x14, 0x22, 0x41, 0x00 ,   // <
     0x14, 0x14, 0x14, 0x14, 0x14 ,   // =
     0x00, 0x41, 0x22, 0x14, 0x08 ,   // >
     0x02, 0x01, 0x51, 0x09, 0x06 ,   // ?
     0x32, 0x49, 0x59, 0x51, 0x3E ,   // @ 0x40
     0x7E, 0x11, 0x11, 0x11, 0x7E ,   // A
     0x7F, 0x49, 0x49, 0x49, 0x36 ,   // B
     0x3E, 0x41, 0x41, 0x41, 0x22 ,   // C
     0x7F, 0x41, 0x41, 0x22, 0x1C ,   // D
     0x7F, 0x49, 0x49, 0x49, 0x41 ,   // E
     0x7F, 0x09, 0x09, 0x09, 0x01 ,   // F
     0x3E, 0x41, 0x49, 0x49, 0x7A ,   // G
     0x7F, 0x08, 0x08, 0x08, 0x7F ,   // H
     0x00, 0x41, 0x7F, 0x41, 0x00 ,   // I
     0x20, 0x40, 0x41, 0x3F, 0x01 ,   // J
     0x7F, 0x08, 0x14, 0x22, 0x41 ,   // K
     0x7F, 0x40, 0x40, 0x40, 0x40 ,   // L
     0x7F, 0x02, 0x0C, 0x02, 0x7F ,   // M
     0x7F, 0x04, 0x08, 0x10, 0x7F ,   // N
     0x3E, 0x41, 0x41, 0x41, 0x3E ,   // O
     0x7F, 0x09, 0x09, 0x09, 0x06 ,   // P 0x50
     0x3E, 0x41, 0x51, 0x21, 0x5E ,   // Q
     0x7F, 0x09, 0x19, 0x29, 0x46 ,   // R
     0x46, 0x49, 0x49, 0x49, 0x31 ,   // S
     0x01, 0x01, 0x7F, 0x01, 0x01 ,   // T
     0x3F, 0x40, 0x40, 0x40, 0x3F ,   // U
     0x1F, 0x20, 0x40, 0x20, 0x1F ,   // V
     0x3F, 0x40, 0x38, 0x40, 0x3F ,   // W
     0x63, 0x14, 0x08, 0x14, 0x63 ,   // X
     0x07, 0x08, 0x70, 0x08, 0x07 ,   // Y
     0x61, 0x51, 0x49, 0x45, 0x43 ,   // Z 0x5A
     0x00, 0x7F, 0x41, 0x41, 0x00 ,   // [
     0x02, 0x04, 0x08, 0x10, 0x20 ,   // '\'
     0x00, 0x41, 0x41, 0x7F, 0x00 ,   // ]
     0x04, 0x02, 0x01, 0x02, 0x04 ,   // ^
     0x40, 0x40, 0x40, 0x40, 0x40 ,   // _
     0x00, 0x01, 0x02, 0x04, 0x00 ,   // '
     0x20, 0x54, 0x54, 0x54, 0x78 ,   // a
     0x7F, 0x48, 0x44, 0x44, 0x38 ,   // b
     0x38, 0x44, 0x44, 0x44, 0x20 ,   // c
     0x38, 0x44, 0x44, 0x48, 0x7F ,   // d
     0x38, 0x54, 0x54, 0x54, 0x18 ,   // e
     0x08, 0x7E, 0x09, 0x01, 0x02 ,   // f
     0x0C, 0x52, 0x52, 0x52, 0x3E ,   // g
     0x7F, 0x08, 0x04, 0x04, 0x78 ,   // h
     0x00, 0x44, 0x7D, 0x40, 0x00 ,   // i
     0x20, 0x40, 0x44, 0x3D, 0x00 ,   // j
     0x7F, 0x10, 0x28, 0x44, 0x00 ,   // k
     0x00, 0x41, 0x7F, 0x40, 0x00 ,   // l
     0x7C, 0x04, 0x18, 0x04, 0x78 ,   // m
     0x7C, 0x08, 0x04, 0x04, 0x78 ,   // n
     0x38, 0x44, 0x44, 0x44, 0x38 ,   // o
     0x7C, 0x14, 0x14, 0x14, 0x08 ,   // p
     0x08, 0x14, 0x14, 0x18, 0x7C ,   // q
     0x7C, 0x08, 0x04, 0x04, 0x08 ,   // r
     0x48, 0x54, 0x54, 0x54, 0x20 ,   // s
     0x04, 0x3F, 0x44, 0x40, 0x20 ,   // t
     0x3C, 0x40, 0x40, 0x20, 0x7C ,   // u
     0x1C, 0x20, 0x40, 0x20, 0x1C ,   // v
     0x3C, 0x40, 0x30, 0x40, 0x3C ,   // w
     0x44, 0x28, 0x10, 0x28, 0x44 ,   // x
     0x0C, 0x50, 0x50, 0x50, 0x3C ,   // y
     0x44, 0x64, 0x54, 0x4C, 0x44 ,   // z
};

uint8_t TEXT_PrintChar(uint8_t x, uint8_t y, uint8_t c){
    const uint8_t *f_data = default_font + (c - ' ') * FONT_W;
	LCD_setPos(x,y);
	for(uint8_t i = 0; i < FONT_W; i++, f_data++){
		LCD_Data(*f_data);
	}
	return x + FONT_W + FONT_SPACING;
}

uint8_t TEXT_Print(uint8_t x, uint8_t y, char *text){
	while(*text)
		x = TEXT_PrintChar(x,y,*(uint8_t*)text++);
	return x;
}

#define CHAR_BUF_LEN   10
uint8_t TEXT_PrintInt(uint8_t x, uint8_t y, int32_t value, int8_t ndig){
    uint8_t c, r, sgn = 0;
	uint8_t buf[CHAR_BUF_LEN], i = 0, pad;
	uint32_t v;
    int8_t radix = 10;

	if (radix < 0) {
		radix = -radix;
		if (value < 0) {
			value = -value;
			sgn = '-';
		}
	}
	v = value;
	r = radix;

    if (ndig < 0) {
		ndig = -ndig;
		pad = '0';
	}else{
        pad = ' ';
    } 

    if (ndig > CHAR_BUF_LEN) {
		ndig = CHAR_BUF_LEN;
	}
   
    ndig = CHAR_BUF_LEN - 1 - ndig;
	i = CHAR_BUF_LEN;
	buf[--i] = '\0';
	
	do {
		c = (uint8_t)(v % r);
		if (c >= 10) c += 7;
		c += '0';
		buf[--i] = c;
		v /= r;
	} while (v);

	if (sgn) buf[--i] = sgn;

    while (i > ndig) {
		buf[--i] = pad;
	}

    ndig = CHAR_BUF_LEN - 1 - i;
	
	while(buf[i]){
		TEXT_PrintChar(x, y, buf[i++]);
        x += FONT_W + FONT_SPACING;
    }
    
    return x;
}



