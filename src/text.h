/*
 * font.h
 *
 *  Created on: 30/03/2020
 *      Author: hmr
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdint.h>

uint8_t TEXT_PrintChar(uint8_t x, uint8_t y, uint8_t c);
uint8_t TEXT_Print(uint8_t x, uint8_t y, char *text);
/**
 * @brief Prints signed integer on display
 * 
 * @param x         X position 127-0
 * @param y         Y Line 7-0
 * @param value     Integer value
 * @param ndig      padding 
 *                  0: No padding
 *                  < 0: Right justified pad with n '0'
 *                  > 0: Right justified pad with n ' '
 * @return uint8_t 
 */
uint8_t TEXT_PrintInt(uint8_t x, uint8_t y, int32_t value, int8_t ndig);

#endif

