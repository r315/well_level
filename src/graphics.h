#ifndef _graphics_h_
#define _graphics_h_

#include <stdint.h>

#define TANK_POS        0,0
#define TANK_W          45
#define TANK_H          64
#define TANK_TOP        18      // y value for graphic

#define PERCENT_POS     104,0
#define AVAILABLE_POS   55,1
#define CAPACITY_POS    93,4
#define LEVEL_LINE      3
#define LEVEL_POS       103,LEVEL_LINE
#define ALERT_COL       (128 - 11)
#define ALERT_POS       ALERT_COL,(64-11)

#define LINE1_TEXT_POS  50,0
#define LINE2_TEXT_POS  50,1
#define LINE3_TEXT_POS  50,2
#define LINE4_TEXT_POS  50,3
#define LINE5_TEXT_POS  50,4
#define LINE6_TEXT_POS  50,5
#define LINE7_TEXT_POS  50,6
#define LINE8_TEXT_POS  50,7

#define NO_COLOR        0
#define FILLED          1

extern const uint8_t tank[];
extern const uint8_t alert[];
extern uint8_t fbuf[];

/**
 * @param x : X coodinate
 * @param y : Y coodinate
 * @param bitmap : pointer to bitmap data
 * @param w : bitmap width
 * @param h : bitmap height
 * */
void drawBitmap(uint16_t x, uint16_t y, uint8_t *bitmap, uint8_t w, uint8_t h);

/**
 * @param level : Graphic level 0-100
 * */
void drawLevel(uint8_t level);

/**
 * @param  
 * */
void drawHline(uint8_t x, uint8_t y, uint8_t size);

/**
 * @param  
 * */
void drawVline(uint8_t x, uint8_t y, uint8_t size);


/**
 * @param
 * */
void printAlert(uint8_t visible);

/**
 * @param distance : distance in mm from bottom to water level
 * */
void printDistance(int32_t distance);

/**
 * @param level : 0-100
 * */
void printPercent(uint8_t level);

/**
 * @param avail : Liters
 * */
void printAvalilable(uint32_t max, uint8_t level);

/**
 * @param cap : Total capacity in liters
 * */
void printCapacity(uint32_t cap);
void printSensor(uint16_t data);

void initGraphics(void);

#endif