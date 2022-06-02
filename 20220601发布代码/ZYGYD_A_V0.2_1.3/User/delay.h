
#ifndef __DELAY_H
#define __DELAY_H  

#include "stm32f10x.h"

//extern void mpu_get_data();
extern void DELAY_Init(void);
extern void DELAY_Ms(uint16_t nms);
extern void DELAY_Us(uint32_t nus);

#endif
