
#include <stddef.h>
#include "board.h"
#include "jsn-sr04t.h"
#include "graphics.h"

#define QUERY_INTERVAL  1000
#define MAX_TIMERS      4

#define true 	1
#define false	0

/**
 * +---------- # ---+  <- Sensor
 * |                |
 * |~~~~~~~~~~~~~~~~|  <- Maximum level
 * |                |
 * |                |
 * |                |
 * |                |
 * |                |
 * |________________|
 * 
 * */

/* Well dimensions and sensor location */
#define DIAMETER        1.5f          // meters
#define HIGHT           2.5f          // meters
#define MAX_HIGHT       466           // distance from bottom to max level in mm
#define SENSOR_OFFSET   206           // distance from max level to sensor in mm

#define RADIUS          (DIAMETER / 2)
#define PI              (3.1415)

typedef struct {
	uint32_t interval;
    uint32_t start;
	void (*func)(void*);
	void *param;
}timer_t;

timer_t timers[MAX_TIMERS];

volatile uint8_t jsn_rdy;
static jsnframe_t jsn_frame;
static uint8_t jsn_error;
static float capacity;
static int32_t distance;

/**
 * 
 * */
uint32_t setTimer(void(*func)(void*), void *param, uint32_t interval){
timer_t *pt = timers;
    for(uint32_t i = 0; i < MAX_TIMERS; i++, pt++){
        if(pt->interval == 0){
            pt->func = func;
            pt->param = param;
            pt->start = getTicks();
            pt->interval = interval;
            return i;
        }
    }
    return MAX_TIMERS;
}

void stopTimer(uint32_t tim){
    timers[tim].interval = 0;
    timers[tim].func = NULL;
}

void sensorResponse(void){
    jsn_rdy = JSN_Checksum(&jsn_frame);
}

void querySensor(void *prt){
    uint8_t data = JSN_START_RANGING;
    int32_t diff, level;

    LED_TOGGLE;
    if(jsn_rdy){
        // print data
        jsn_rdy = false;
        distance = JSN_Distance(&jsn_frame) - SENSOR_OFFSET;   
       
        diff = MAX_HIGHT - distance ;

        // Only positive differences are valid
        if(diff >= 0){
            level = diff * 100 / MAX_HIGHT;

            printPercent(level);
            printAvalilable((capacity * level) / 100);
            drawLevel(level);
            printDistance(diff);
        }

        if(jsn_error == true){
            jsn_error = false;
            printAlert(false);
        }
    }else{
        if(jsn_error == false){
            jsn_error = true;
            printAlert(true);
        }
    }
    BOARD_UsartReceiveDMA((uint8_t*)&jsn_frame, JSN_FRAME_SIZE, sensorResponse);
    BOARD_UsartTransmit(&data, 1);
}

void processTimers(void){
uint32_t now = getTicks();
timer_t *pt = timers;
    for(uint32_t i = 0; i < MAX_TIMERS; i++, pt++){
        if(pt->interval > 0){
            uint32_t diff = now - pt->start;
            if(diff >= pt->interval){
                pt->func(pt->param);
                pt->start = now;
            }
        }
    }
}

void update(void *p){
uint8_t level = 0;

int32_t diff = MAX_HIGHT - (distance - SENSOR_OFFSET);

    level = diff * 100 / MAX_HIGHT;

    printPercent(level);
    printAvalilable((capacity * level) / 100);
    drawLevel(level);
    printDistance(diff);

    distance -= 50;
    if(distance < 0){
        distance = MAX_HIGHT;
    }
}

int main(void){

    LCD_Init();      

    serialInit(JSN_SERIAL_SPEED);

    setTimer(querySensor, NULL, QUERY_INTERVAL);
    BOARD_UsartReceiveDMA((uint8_t*)&jsn_frame, JSN_FRAME_SIZE, sensorResponse);
    
    xmemset(fbuf, 0x0, 8*128);
   
    drawHline(50, 18, 128 - 50);
    drawHline(50, 42, 128 - 50);
    drawVline(48, 0, 64);    
    drawBitmap(TANK_POS,(uint8_t*)tank, TANK_W, TANK_H);
    
    capacity = PI*RADIUS*RADIUS*HIGHT;  // cubic meters
    capacity = capacity * 1000;         // Liters

    printCapacity(capacity);

    TEXT_Print(LINE1_TEXT_POS, "Disp.:");
    TEXT_Print(LINE4_TEXT_POS, "Altura:");
    TEXT_Print(LINE5_TEXT_POS, "Total:");

    distance = MAX_HIGHT;

    //setTimer(update, NULL, 500);

    while(1){
        processTimers();
    }
	return 0;
}
