#include "board.h"
#include "jsn-sr04t.h"

static uint32_t speed_of_sound = 3432UL;  // 343.2 m/s

uint8_t JSN_Checksum(jsnframe_t *frame){
    uint8_t sum = 0, *ptr = (uint8_t*)frame;

	for(uint16_t i = 0; i < JSN_FRAME_SIZE - 1; i++, ptr++){
		sum += *ptr;
	}

	return !(sum - frame->sum);
}

uint16_t JSN_Distance(jsnframe_t *frame){
	#if JSN_MODE == JSN_MODE1
	uint32_t time = (frame->h_data << 8) | frame->l_data;
	// formula from DS d = (time * speed of sound) / 2
	return (time * speed_of_sound) / 20000;
	#else
	return (frame->h_data << 8) | frame->l_data;
	#endif
}

void JSN_Trigger(){
	#if JSN_MODE == JSN_MODE1
	BOARD_GpioWrite(BOARD_JSN_TRI_PORT, BOARD_JSN_TRI_PIN, 1);
	BOARD_DelayMs(1);
	BOARD_GpioWrite(BOARD_JSN_TRI_PORT, BOARD_JSN_TRI_PIN, 0);
	#elif JSN_MODE == JSN_MODE3
	BOARD_UartTransmit((uint8_t []){JSN_START_RANGING}, 1);
	#endif
}