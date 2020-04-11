/*
 * font.h
 *
 *  Created on: 30/03/2020
 *      Author: hmr
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdint.h>

#define HIT_FONT_W 10
#define HIT_FONT_H 2
#define HITACHI_FONT_LINE_BYTES HIT_FONT_W * HIT_FONT_H

extern const uint8_t default_font[];

uint8_t TEXT_PrintChar(uint8_t x, uint8_t y, uint8_t c);
uint8_t TEXT_PrintInt(uint8_t x, uint8_t y, int32_t value, int8_t ndig);
uint8_t TEXT_Print(uint8_t x, uint8_t y, char *text);

#endif

