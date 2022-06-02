#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
 
 
/***************************CLOCK*****************************/
//�Ƿ��ʱ�ӵδ���Clock ticker
#define CLOCKTICKER														0

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

void TimeStamp_Increment(void);
void get_tick_count(unsigned long *count);


//ʱ�ӹ�����
typedef struct
{
	bool isInit;
	/**
		* @brief  ʱ�ӳ�ʼ������
		* @param  u32 ��ʱʱ�䵥λms
		* @retval : None
	**/
	void (*HAL_ClockInit)(void);
	/**
		* @brief  ��ʱ����
		* @param  u32 ��ʱʱ�䵥λms
		* @retval : None
	**/
	void (*HAL_DelayMS)(u16);
	
	/**
		* @brief  ��ʱ����
		* @param  u32 ��ʱʱ�䵥λus
		* @retval : None
	**/
	void (*HAL_DelayUS)(u32);
	/**
		* @brief  ��ʱ����
		* @param  u32 ��ʱʱ�䵥λus
		* @retval : None
	**/
	void (*HAL_Get_TickMS)(unsigned long*);
}HalAdapter_Management_Clock_t;

extern HalAdapter_Management_Clock_t halAdapter_Clock;
#endif



