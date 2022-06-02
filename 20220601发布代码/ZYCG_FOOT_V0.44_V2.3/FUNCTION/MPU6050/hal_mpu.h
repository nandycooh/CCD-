#ifndef __HAL_MPU_H
#define __HAL_MPU_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "stdbool.h"

//������
typedef struct
{
	bool isInit;
	/**
		* @brief  ��ʼ��
		* @param  None
		* @retval : None
	**/
	void (*HAL_MPU_Init)(void);
	/**
		* @brief  �����ж�
		* @param  None
		* @retval : None
	**/
	void (*HAL_MPU_EnableInterrupt)(void);
	/**
		* @brief  �ر��ж�
		* @param  None��
		* @retval : None
	**/
	void (*HAL_MPU_DisableInterrupt)(void);
	
	/**
		* @brief  �Ƿ����ж�
		* @param  None
		* @retval : false��û�з����ж�, true�������ж�
	**/
	bool (*HAL_MPU_GetInterrupStatus)(void);
	
}HalAdapter_Management_MPU_t;

HalAdapter_Management_MPU_t* GetHalAdapter_MPU(void);

#endif
