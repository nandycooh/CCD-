#ifndef __POWERMANAGE_H
#define __POWERMANAGE_H

#include "stm32f10x.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"

#define BAT_CHARGE_PIN 	GPIO_Pin_11    //�ڲ���USB���жϣ�����У���   ��������
#define USB_CHECK_PIN 	GPIO_Pin_12		 //USB��Դ�ܽ�
#define BAT_CHRG_IN     PBin(12)
#define USB_CHECK_IN    PBin(10)
#define KEY_02			PBin(3)
#define	POWER_ON_OFF	PBout(4)

#define READBAT  GPIO_ReadInputDataBit(GPIOB, BAT_CHARGE_PIN)//���ʱ��״̬
#define READUSB  GPIO_ReadInputDataBit(GPIOB, USB_CHECK_PIN)//�Ƿ����USB


/***************************POWER******************************/
#define BAT_CAPACITY_LV1   4.000000      //��Ӧ��ʾ����80%
#define BAT_CAPACITY_LV2   3.850000     //��Ӧ��ʾ����60%
#define BAT_CAPACITY_LV3   3.770000     //��Ӧ��ʾ����40%
#define BAT_CAPACITY_LV4   3.720000     //��Ӧ��ʾ����20%
#define BAT_CAPACITY_LV5   3.600000      //��Ӧ��ʾ������
#define BAT_CAPACITY_LV6   3.300000      //��Ӧǿ�ƹػ�����
//#define BAT_CAPACITY_LV1   1.35     //��Ӧ��ʾ����80%
//#define BAT_CAPACITY_LV2   1.30     //��Ӧ��ʾ����60%
//#define BAT_CAPACITY_LV3   1.22    //��Ӧ��ʾ����40%
//#define BAT_CAPACITY_LV4   1.15     //��Ӧ��ʾ����20%
//#define BAT_CAPACITY_LV5   1.10     //��Ӧ��ʾ������
//#define BAT_CAPACITY_LV6   1.06     //��Ӧǿ�ƹػ�����


u8 Power_Shut_Down(u8 shutime);

//IIC������
typedef struct
{
	bool isInit;
	/**
		* @brief  IIC��ʼ��
		* @param  None
		* @retval : None
	**/
	void (*HAL_Power_Init)(void);
	/**
		* @brief  �������ڲ���ж�
		* @param  None
		* @retval : None
	**/
	void (*HAL_PowerCharge_EnableInterrupt)(void);
	/**
		* @brief  �������ڲ���ж�
		* @param  None��
		* @retval : None
	**/
	void (*HAL_PowerCharge_DisableInterrupt)(void);
	/**
		* @brief  ��ȡ����״̬
		* @param  None
		* @retval : 0������δ���� 1�����ڲ���
	**/
	uint8_t (*HAL_Get_ChargePinStatus)(void);
	/**
		* @brief  ��ȡ�Ƿ��������(�����Ǵ��ڳ��״̬��)
		* @param  None
		* @retval :���ϳ���ʱ��0��δ������ 1�������磩 δ���ϳ��ڣ�ֻ����1��
	**/
	uint8_t (*HAL_Get_FullBatteryPin_InCharge)(void);
	
	uint8_t (*HAL_Get_battery_power)(void);
	
}HalAdapter_Management_Power_t;


#endif
