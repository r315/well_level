#include <stddef.h>
#include "board.h"
#include "timer.h"

static timer_t timers[MAX_TIMERS];


/**
 * @brief Configure a sw timer for continuously call a function.
 * 
 * \param func : pointer to the function to be called when timer expires
 * \param param : pointer to function parameters
 * \param interval : function calling interval in ms
 * \return : timer index on success, MAX_TIMERS if no timer is available
 * */
uint32_t TIMER_Interval(void(*func)(void*), void *param, uint32_t interval){
timer_t *pt = timers;
    for(uint32_t id = 0; id < MAX_TIMERS; id++, pt++){
        if(pt->interval == 0){
            pt->func = func;
            pt->param = param;
            pt->start = BOARD_GetTicks();
            pt->interval = interval;
            return id;
        }
    }
    return MAX_TIMERS;
}

void TIMER_Cancel(uint32_t id){
    timers[id].interval = 0;
    timers[id].func = NULL;
}

/**
 * @brief Loop through all timers checking if expired and call
 * the correspondent function. This should be called from main 
 * loop or OS task.
 * */
void TIMER_Process(void){
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
