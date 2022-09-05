
#include <stddef.h>
#include "board.h"
#include "jsn-sr04t.h"
#include "graphics.h"
#include "timer.h"

#define QUERY_INTERVAL  1000

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
#define TANK_DIAMETER       1500          // mm
#define TANK_HIGHT          2500          // mm
#define TANK_RADIUS         (TANK_DIAMETER / 2)
#define TANK_CAPACITY       ((PI * TANK_RADIUS * TANK_RADIUS * TANK_HIGHT) / 1000000UL) // cubic mm * 1000L

#define SENSOR_OFFSET       210                     // distance from max level to sensor in mm
#define SENSOR_MAX_DISTANCE (SENSOR_OFFSET + TANK_HIGHT)  // distance from bottom to max level in mm

#define PI                  (3.1415)

volatile uint8_t jsn_rdy;
static jsnframe_t jsn_frame;

#if JSN_MODE
/**
 * @brief Callback from USART interrupt that 
 * indicate if a frame was properly received.
 * */
static void sensorResponse(uint32_t time){
    #if JSN_MODE == JSN_MODE1
    jsn_frame.h_data = (time >> 8) & 255;
    jsn_frame.l_data = time & 255;
    jsn_rdy = true;
    #elif JSN_MODE == JSN_MODE3
    jsn_rdy = JSN_Checksum(&jsn_frame);
    #endif
    LED_ON;
}

/**
 * @brief Callback from sw timer every second to read sensor
 * 
 * \param ptr : pointer to extra parameters, not used
 *
 * */
static void querySensor(void *prt){
    uint8_t level, jsn_error = true;
    int32_t diff;
    int32_t raw_measure;

    if(jsn_rdy){
        jsn_rdy = false;        
       
        raw_measure = JSN_Distance(&jsn_frame);

        #if DEBUG
        printSensor(raw_measure);
        #endif

        if(raw_measure > JSN_MIN_DIST && raw_measure < JSN_MAX_DIST){
            raw_measure -= SENSOR_OFFSET;
            diff = TANK_HIGHT - raw_measure;

            printDistance(diff);

            // If negative the most probable is that level is above TANK_HIGHT
            if(diff < 0){
                diff = TANK_HIGHT;
            }
            
            level = (diff * 100) / TANK_HIGHT;

            printAvalilable(TANK_CAPACITY, level);
            drawLevel(level);

            jsn_error = false;
	    }
    }

    printAlert(jsn_error);

    #if JSN_MODE == JSN_MODE1
    BOARD_MeasurePulse(GPIO_HIGH, sensorResponse);
    #elif JSN_MODE == JSN_MODE3
    // Prepare receive
    BOARD_UartReceiveDMA((uint8_t*)&jsn_frame, JSN_FRAME_SIZE, sensorResponse);
    #endif
    // Trigger sensor
    JSN_Trigger();
    LED_OFF;
}
#else
/**
 * @brief Callback from sw timer for creating a animated
 * demo
 * 
 * \param ptr : pointer to extra parameters, not used
 *
 * */
static void demo(void *ptr){
    uint8_t level;
    static uint16_t raw_measure = SENSOR_MAX_DISTANCE + SENSOR_OFFSET;

    raw_measure -= 50;
    if(raw_measure < SENSOR_OFFSET){
        raw_measure = SENSOR_MAX_DISTANCE + SENSOR_OFFSET;
    }

    int32_t diff = SENSOR_MAX_DISTANCE - (raw_measure - SENSOR_OFFSET);

    level = (diff * 100) / SENSOR_MAX_DISTANCE;

    printAvalilable(TANK_CAPACITY, level);
    drawLevel(level);
    printDistance(diff);
}
#endif
/**
 * @brief Main entry point
 * */
int main(void){

    BOARD_Init();
    
    LCD_Init();      

    initGraphics();
    printCapacity(TANK_CAPACITY);

#if JSN_MODE
    TIMER_Interval(querySensor, NULL, QUERY_INTERVAL);
#else
    TIMER_Interval(demo, NULL, QUERY_INTERVAL);
#endif

    while(1){
        TIMER_Process();
    }

	return 0;
}
