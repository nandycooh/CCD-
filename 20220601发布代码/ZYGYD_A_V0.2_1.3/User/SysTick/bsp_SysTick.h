#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

int get_tick_count(unsigned long *count);
void mdelay(unsigned long nTime);
void SysTick_Init(void);
void DELAY_Ms(uint16_t nms);


#endif /* __SYSTICK_H */
