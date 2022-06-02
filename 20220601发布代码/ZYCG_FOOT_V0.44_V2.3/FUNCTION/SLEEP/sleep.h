#ifndef __SLEEP_H
#define __SLEEP_H

#include "sys.h"
#include "delay.h"

//����ģʽ������
typedef struct
{
	bool isInit;
	/**
		* @brief  ����Stopģʽ����
		* @param  None
		* @retval : None
	**/
	void (*HAL_EnterStop)(void);
	/**
		* @brief  ����LowPowerStopģʽ����
		* @param  None
		* @retval : None
	**/
	void (*HAL_EnterLowPowerStop)(void);
	/**
		* @brief  ����Standbyģʽ����
		* @param  None
		* @retval : None
	**/
	void (*HAL_EnterStandby)(void);
	/**
		* @brief  �ָ�Ĭ������
		* @param  None
		* @retval : None
	**/
	void (*HAL_RestoreDefaultSettings)(void);
	/**
		* @brief  ��������
		* @param  None
		* @retval : None
	**/
	void (*HAL_Restart)(void);
}HalAdapter_Management_OperatingMode_t;

HalAdapter_Management_OperatingMode_t* GetHalAdapter_OptMode(void);

void Un_Used_Gpio_Init(void);
#endif
