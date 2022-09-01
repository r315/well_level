
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
#define DIAMETER        1.5f          // meters
#define HIGHT           2.5f          // meters
#define MAX_HIGHT       466           // distance from bottom to max level in mm
#define SENSOR_OFFSET   206           // distance from max level to sensor in mm

#define RADIUS          (DIAMETER / 2)
#define PI              (3.1415)

#define CAPACITY        (PI*RADIUS*RADIUS*HIGHT * 1000) // cubic meters * 1000L

volatile uint8_t jsn_rdy;
static jsnframe_t jsn_frame;
static uint8_t jsn_error;

#if JSN_MODE == JSN_MODE1
static void triggerSensor(void *ptr){
    JSN_Trigger();
}

#elif JSN_MODE == JSN_MODE3
/**
 * @brief Callback from USART interrupt that 
 * indicate if a frame was properly received.
 * */
static void sensorResponse(void){
    jsn_rdy = JSN_Checksum(&jsn_frame);
}

/**
 * @brief Callback from sw timer every second to read sensor
 * 
 * \param ptr : pointer to extra parameters, not used
 *
 * */
static void querySensor(void *prt){    
    int32_t diff, level;
    uint16_t raw_measure;
    int32_t distance;

    LED_TOGGLE;
    if(jsn_rdy){
        // print data
        jsn_rdy = false;
        raw_measure = JSN_Distance(&jsn_frame);

        #if 1
        TEXT_PrintInt(50, 7, raw_measure, 4);
        #endif
       
        distance =  raw_measure - SENSOR_OFFSET;
        diff = MAX_HIGHT - distance ;

        // Only positive differences are valid
        if(diff >= 0){
            level = diff * 100 / MAX_HIGHT;

            printPercent(level);
            printAvalilable((CAPACITY * level) / 100);
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
    // Prepare receive
    BOARD_UartReceiveDMA((uint8_t*)&jsn_frame, JSN_FRAME_SIZE, sensorResponse);
    // Trigger sensor
    JSN_Trigger();
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
    static int32_t distance = MAX_HIGHT;

    int32_t diff = MAX_HIGHT - (distance - SENSOR_OFFSET);

    level = diff * 100 / MAX_HIGHT;

    printPercent(level);
    printAvalilable((CAPACITY * level) / 100);
    drawLevel(level);
    printDistance(diff);

    distance -= 50;
    if(distance < 0){
        distance = MAX_HIGHT;
    }
}
#endif
/**
 * @brief Main entry point
 * */
int main(void){

    BOARD_Init();
#if JSN_MODE == JSN_MODE1    
    TIMER_Interval(triggerSensor, NULL, 50);
#else 
    
    LCD_Init();      

#if JSN_MODE == JSN_MODE3
    TIMER_Interval(querySensor, NULL, QUERY_INTERVAL);
#else
    TIMER_Interval(demo, NULL, 500);
#endif
    
    initGraphics();
   
    drawHline(50, 18, 128 - 50);
    drawHline(50, 42, 128 - 50);
    drawVline(48, 0, 64);    
    drawBitmap(TANK_POS,(uint8_t*)tank, TANK_W, TANK_H);
    
    TEXT_Print(LINE1_TEXT_POS, "Disp.:");
    TEXT_Print(LINE4_TEXT_POS, "Altura:");
    TEXT_Print(LINE5_TEXT_POS, "Total:");

    printCapacity(CAPACITY);
#endif

    while(1){
        TIMER_Process();
    }

	return 0;
}
