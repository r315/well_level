#include "jsn-sr04t.h"

uint8_t JSN_Checksum(jsnframe_t *frame){
    uint8_t sum = 0, *ptr = (uint8_t*)frame;

	for(uint16_t i = 0; i < JSN_FRAME_SIZE - 1; i++, ptr++){
		sum += *ptr;
	}

	return !(sum - frame->sum);
}

uint16_t JSN_Distance(jsnframe_t *frame){
    return (frame->h_data << 8) | frame->l_data;
}