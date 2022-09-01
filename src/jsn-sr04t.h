/*
 * jsn-sr04t.h
 *
 *  Created on: Mar 30, 2020
 *      Author: hmr
 */

#ifndef SRC_JSN_SR04T_H_
#define SRC_JSN_SR04T_H_

#include <stdint.h>

#define JSN_START_RANGING		0x55
#define JSN_FRAME_SIZE		    0x04

#define JSN_SERIAL_SPEED        9600

#define JSN_MODE1               1
#define JSN_MODE2               2
#define JSN_MODE3               3

typedef struct{
    uint8_t header;
    uint8_t h_data;
    uint8_t l_data;
    uint8_t sum;
}jsnframe_t;

uint8_t JSN_Checksum(jsnframe_t *frame);
uint16_t JSN_Distance(jsnframe_t *frame);

void JSN_Trigger();

#endif /* SRC_JSN_SR04T_H_ */
