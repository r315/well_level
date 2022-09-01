#ifndef _timer_h_
#define _timer_h_

#include <stdint.h>


typedef struct {
	uint32_t interval;
    uint32_t start;
	void (*func)(void*);
	void *param;
}timer_t;

#define MAX_TIMERS      4

uint32_t TIMER_Interval(void(*func)(void*), void *param, uint32_t interval);
void TIMER_Process(void);
void TIMER_Cancel(uint32_t id);

#endif